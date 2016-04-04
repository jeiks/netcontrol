import os.path

from netcontrol.com import *
from netcontrol.api import URLHandler, url
from netcontrol.utils import wsgi_serve_file
from netcontrol.plugmgr import PluginLoader


class Downloader(URLHandler, Plugin):

    @url('^/dl/.+/.+')
    def process_dl(self, req, start_response):
        params = req['PATH_INFO'].split('/', 3)
        self.log.debug('Dispatching download: %s'%req['PATH_INFO'])

        path = PluginLoader.get_plugin_path(self.app, params[2])
        file = os.path.join(path, params[2], 'files', params[3])

        return wsgi_serve_file(req, start_response, file)

    @url('^/htdocs/.+')
    def process_htdocs(self, req, start_response):
        params = req['PATH_INFO'].split('/', 2)
        self.log.debug('Dispatching htdocs: %s'%req['PATH_INFO'])

        path = self.app.config.get('netcontrol', 'htdocs')
        file = os.path.join(path, params[2])
        file = os.path.normpath(os.path.realpath(file))

        if not file.startswith(path):
            start_response('404 Not Found', [])
            return ''

        return wsgi_serve_file(req, start_response, file)
