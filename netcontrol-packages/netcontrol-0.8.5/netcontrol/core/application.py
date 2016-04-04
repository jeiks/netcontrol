from netcontrol.plugins import *
from netcontrol.utils import *
from netcontrol.ui import *
from netcontrol.plugmgr import PluginLoader
import netcontrol.ui.xslt as xslt
import netcontrol


from session import *
from auth import *


class Application (PluginManager, Plugin):
    """
    Class representing app state during a request.
    Instance vars:

    - ``config`` - :class:`netcontrol.config.ConfigProxy` - config for the current user
    - ``gconfig`` - :class:`netcontrol.config.Config` - global app config
    - ``auth`` - :class:`netcontrol.core.AuthManager` - authentication system
    - ``log`` - :class:`logging.Logger` - app log
    - ``session`` - ``dict`` - full access to the session
    """

    def __init__(self, config=None):
        PluginManager.__init__(self)
        self.gconfig = config
        self.log = config.get('log_facility')
        self.platform = config.get('platform')
        PluginLoader.register_observer(self)
        self.refresh_plugin_data()

    def plugins_changed(self):
        """
        Implementing PluginLoader observer
        """
        self.refresh_plugin_data()

    def refresh_plugin_data(self):
        """
        Rescans plugins for JS, CSS, LESS, XSLT widgets and XML templates.
        """
        self.template_path = []
        self.less_styles = []
        self.template_styles = []
        self.template_scripts = []
        self.layouts = {}
        includes = []
        functions = {}

        for f in self.grab_plugins(IXSLTFunctionProvider):
            functions.update(f.get_funcs())

        # Get path for static content and templates
        plugins = []
        plugins.extend(PluginLoader.list_plugins().keys())
        plugins.extend(netcontrol.plugins.plist)

        for c in plugins:
            path = os.path.join(PluginLoader.get_plugin_path(self, c), c)

            fp = os.path.join(path, 'files')
            if os.path.exists(fp):
                self.template_styles.extend([
                    '/dl/'+c+'/'+s
                    for s in os.listdir(fp)
                    if s.endswith('.css')
                ])
                self.less_styles.extend([
                    '/dl/'+c+'/'+s
                    for s in os.listdir(fp)
                    if s.endswith('.less')
                ])
                self.template_scripts.extend([
                    '/dl/'+c+'/'+s
                    for s in os.listdir(fp)
                    if s.endswith('.js')
                ])

            wp = os.path.join(path, 'widgets')
            if os.path.exists(wp):
                includes.extend([
                    os.path.join(wp, s)
                    for s in os.listdir(wp)
                    if s.endswith('.xslt')
                ])

            lp = os.path.join(path, 'layout')
            if os.path.exists(lp):
                for s in os.listdir(lp):
                    if s.endswith('.xml'):
                        self.layouts['%s:%s'%(c,s)] = os.path.join(lp, s)

            tp = os.path.join(path, 'templates')
            if os.path.exists(tp):
                self.template_path.append(tp)

        if xslt.xslt is None:
            xslt.prepare(
                includes,
                functions
            )

    @property
    def config(self):
        if hasattr(self, 'auth'):
            return self.gconfig.get_proxy(self.auth.user)
        else:
            return self.gconfig.get_proxy(None)

    def start_response(self, status, headers=[]):
        self.status = status
        self.headers = headers

    def fix_length(self, content):
        # TODO: maybe move this method to middleware
        has_content_length = False
        for header, value in self.headers:
            if header.upper() == 'CONTENT-LENGTH':
                has_content_length = True
        if not has_content_length:
            self.headers.append(('Content-Length',str(len(content))))

    def dispatcher(self, environ, start_response):
        """
        Dispatches WSGI requests
        """
        self.log.debug('Dispatching %s'%environ['PATH_INFO'])
        self.environ = environ
        self.status = '200 OK'
        self.headers = [('Content-type','text/html')]
        self.session = environ['app.session']

        content = 'Sorry, no content for you'
        for handler in self.grab_plugins(IURLHandler):
            if handler.match_url(environ):
                try:
                    self.log.debug('Calling handler for %s'%environ['PATH_INFO'])
                    content = handler.url_handler(self.environ,
                                                  self.start_response)
                except Exception, e:
                    #print traceback.format_exc()
                    try:
                        content = format_error(self, e)
                    except:
                        status = '418 I\'m a teapot'
                        content = 'Fatal error occured:\n' + traceback.format_exc()
                finally:
                    break

        start_response(self.status, self.headers)
        self.fix_length(content)
        content = [content]
        self.log.debug('Finishing %s'%environ['PATH_INFO'])
        return content

    def plugin_enabled(self, cls):
        return self.platform.lower() in [x.lower() for x in cls.platform] \
           or 'any' in cls.platform

    def plugin_activated(self, plugin):
        plugin.log = self.log
        plugin.app = self

    def grab_plugins(self, iface, flt=None):
        """
        Returns list of available plugins for given interface, optionally filtered.

        :param  iface:  interface to match plugins against
        :type   iface:  :class:`netcontrol.com.Interface`
        :param  flt:    filter function
        :type   flt:    func(Plugin)
        :rtype:         list(:class:`netcontrol.com.Plugin`)
        """
        plugins = self.plugin_get(iface)
        plugins = list(set(filter(None, [self.instance_get(cls, True) for cls in plugins])))
        if flt:
            plugins = filter(flt, plugins)
        return plugins

    def get_backend(self, iface, flt=None):
        """
        Same as ``grab_plugins``, but returns the first plugin found and will
        raise :class:`netcontrol.util.BackendRequirementError` if no plugin was
        found.

        :param  iface:  interface to match plugins against
        :type   iface:  :class:`netcontrol.com.Interface`
        :param  flt:    filter function
        :type   flt:    func(Plugin)
        :rtype:         :class:`netcontrol.com.Plugin`
        """
        lst = self.grab_plugins(iface, flt)
        if len(lst) == 0:
            raise BackendRequirementError(iface.__name__)
        return lst[0]

    def get_config(self, plugin):
        """
        Returns :class:`netcontrol.api.ModuleConfig` for a given plugin.
        """
        if plugin.__class__ != type:
            plugin = plugin.__class__
        return self.get_config_by_classname(plugin.__name__)

    def get_config_by_classname(self, name):
        """
        Returns :class:`netcontrol.api.ModuleConfig` for a given plugin class name.
        """
        cfg = self.get_backend(IModuleConfig,
                flt=lambda x: x.target.__name__==name)
        cfg.overlay_config()
        return cfg

    def get_template(self, filename=None, search_path=[]):
        return BasicTemplate(
                filename=filename,
                search_path=self.template_path + search_path,
                styles=self.template_styles,
                scripts=self.template_scripts
               )

    def inflate(self, layout):
        """
        Inflates an XML UI layout into DOM UI tree.

        :param  layout: '<pluginid>:<layoutname>', ex: dashboard:main for
                        /plugins/dashboard/layout/main.xml
        :type   layout: str
        :rtype:         :class:`netcontrol.ui.Layout`
        """
        f = self.layouts[layout+'.xml']
        return Layout(f)

    def stop(self):
        """
        Exits Netcontrol
        """
        from os import getpid
        pidFile = '/var/run/netcontrol.pid'
        pidNumber = int( open( pidFile ).read().replace('\n','').replace(' ','') )
        if pidNumber <> getpid():
            return None

        if os.path.exists( pidFile ):
            os.unlink( pidFile )
        self.config.get('server').stop()

    def restart(self):
        """
        Restarts Netcontrol process
        """
        self.config.get('server').restart_marker = True
        self.stop()


class AppDispatcher(object):
    """
    Main WSGI dispatcher which assembles session, auth and application
    altogether
    """
    def __init__(self, config=None):
        self.config = config
        self.log = config.get('log_facility')
        self.sessions = SessionStore.init_safe()

    def dispatcher(self, environ, start_response):
        self.log.debug('Dispatching %s'%environ['PATH_INFO'])

        app = Application(self.config)
        auth = AuthManager(self.config, app, app.dispatcher)
        sm = SessionManager(self.sessions, auth)

        return sm(environ, start_response)
