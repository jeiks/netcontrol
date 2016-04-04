"""
Base plugin-interface architecture for Netcontrol
"""

__all__ = ['Interface', 'implements', 'Plugin', 'PluginManager']

import inspect
import traceback

from netcontrol.utils import PrioList


class Interface:
    """ Base abstract class for all interfaces

    Can be used as callable (decorator)
    to check if Plugin implements all methods
    (internal use only)
    """

    def __call__(self, cls):
        # Check that target class supports all our interface methods
        cls_methods = [m for m in dir(cls) if not m.startswith('_')]

        # Check local interface methods
        methods = [m for m in dir(self.__class__) if not m.startswith('_')]
        # Filter out property methods
        methods = [m for m in methods if m not in dir(property)]

        for method in methods:
            if method not in cls_methods:
                raise AttributeError(
                      "%s implementing interface %s, does not have '%s' method"%
                      (cls, self.__class__, method))


def implements (*interfaces):
    """
    Used to note that a :class:`Plugin` implements an :class:`Interface`.
    Example:

        class IFoo (Interface):
            pass

        class IBaz (Interface):
            pass

        class FooBazImp (Plugin):
            implements (IFoo, IBaz)
    """

    # Get parent exection frame
    frame = inspect.stack()[1][0]
    # Get locals from it
    locals = frame.f_locals

    if ((locals is frame.f_globals) or
        ('__module__' not in locals)):
        raise TypeError('implements() can only be used in class definition')

    if '_implements' in locals:
        raise TypeError('implements() could be used only once')

    locals.setdefault('_implements',[]).extend(interfaces)
    # TODO: trac also all base interfaces (if needed)


class PluginManager (object):
    """ Holds all registered classes, instances and implementations
    You should have one class instantiated from both PluginManager and Plugin
    to trigger plugins magick
    """
    # Class-wide properties
    __classes = []
    __plugins = {}
    __tracking = False
    __tracker = None

    def __init__(self):
        self.__instances = {}

    @staticmethod
    def class_register (cls):
        """
        Registers a new class

        :param  cls:    class
        :type   cls:    type
        """
        PluginManager.__classes.append(cls)
        if PluginManager.__tracking:
            PluginManager.__tracker.append(cls)

    @staticmethod
    def class_unregister (cls):
        """
        Unregisters a class

        :param  cls:    class
        :type   cls:    type
        """
        PluginManager.__classes.remove(cls)
        for lst in PluginManager.__plugins.values():
            if cls in lst:
                lst.remove(cls)

    @staticmethod
    def class_list ():
        """
        Lists all registered classes

        :returns:       list(:class:`type`)
        """
        return PluginManager.__classes

    @staticmethod
    def plugin_list ():
        return PluginManager.__plugins

    @staticmethod
    def plugin_register (iface, cls):
        """
        Registers a :class:`Plugin` for implementing an :class:`Interface`

        :param  iface:  interface
        :type   iface:  type
        :param  cls:    plugin
        :type   cls:    :class:`Plugin`
        """
        lst = PluginManager.__plugins.setdefault(iface,PrioList())
        for item in lst:
            if str(item) == str(cls):
                return
        lst.append(cls)

    @staticmethod
    def plugin_get (iface):
        """
        Returns plugins that implement given :class:`Interface`

        :param  iface:  interface
        :type   iface:  type
        """
        return PluginManager.__plugins.get(iface, [])

    @staticmethod
    def start_tracking():
        """
        Starts internal registration tracker
        """
        PluginManager.__tracking = True
        PluginManager.__tracker = []

    @staticmethod
    def stop_tracking():
        """
        Stops internal registration tracker and returns all classes
        registered since calling ``start_tracking``
        """
        PluginManager.__tracking = False
        return PluginManager.__tracker

    def instance_get(self, cls, instantiate=False):
        """
        Gets a saved instance for the :class:`Plugin` subclass

        :param  instantiate:  instantiate plugin if it wasn't instantiate before
        :type   instantiate:  bool
        """
        if not self.plugin_enabled(cls):
            return None
        inst = self.__instances.get(cls)
        if instantiate == True and inst is None:
            if cls not in PluginManager.__classes:
                raise Exception('Class "%s" is not registered'% cls.__name__)
            try:
                inst = cls(self)
            except TypeError, e:
                print traceback.format_exc()
                raise Exception('Unable instantiate plugin %r (%s)'%(cls, e))

        return inst

    def instance_set(self, cls, inst):
        self.__instances[cls] = inst

    def instance_list(self):
        return self.__instances

    def plugin_enabled(self, cls):
        """
        Called to check if :class:`Plugin` is eligible for running on this system

        :returns: bool
        """
        return True

    def plugin_activated(self, plugin):
        """
        Called when a :class:`Plugin` is successfully instantiated
        """


class MetaPlugin (type):
    """
    Metaclass for Plugin
    """

    def __new__ (cls, name, bases, d):
        """ Create new class """

        # Create new class
        new_class = type.__new__(cls, name, bases, d)

        # If we creating base class, do nothing
        if name == 'Plugin':
            return new_class

        # Override __init__ for Plugins, for instantiation process
        if True not in [issubclass(x, PluginManager) for x in bases]:
            # Allow Plugins to have own __init__ without parameters
            init = d.get('__init__')
            if not init:
                # Because we're replacing the initializer, we need to make sure
                # that any inherited initializers are also called.
                for init in [b.__init__._original for b in new_class.mro()
                             if issubclass(b, Plugin)
                             and '__init__' in b.__dict__]:
                    break
            def maybe_init(self, plugin_manager, init=init, cls=new_class):
                if plugin_manager.instance_get(cls) is None:
                    # Plugin is just created
                    if init:
                        init(self)
                    if not self.multi_instance:
                        plugin_manager.instance_set(cls, self)
            maybe_init._original = init
            new_class.__init__ = maybe_init

        # If this is abstract class, do no record it
        if d.get('abstract'):
            return new_class

        # Save created class for future reference
        PluginManager.class_register(new_class)

        # Collect all interfaces that this class implements
        interfaces = d.get('_implements',[])
        for base in [base for base in new_class.mro()[1:] if hasattr(base, '_implements')]:
            interfaces.extend(base._implements)

        # Delete duplicates, in case we inherit same Intarfaces
        # or we need to override priority
        _ints = []
        _interfaces = []
        for interface in interfaces:
            _int = interface
            if isinstance(interface, tuple):
                _int = interface[0]

            if _int not in _ints:
                _ints.append(_int)
                _interfaces.append(interface)

        interfaces = _interfaces

        # Check that class supports all needed methods
        for interface in interfaces:
            _int = interface
            if isinstance(interface, tuple):
                _int = interface[0]
            _int()(new_class)

        # Register plugin
        for interface in interfaces:
            if isinstance(interface, tuple):
                PluginManager.plugin_register(interface[0], (new_class, interface[1]))
            else:
                PluginManager.plugin_register(interface, new_class)

        return new_class

#class MetaPlugin


class Plugin (object):
    """
    Base class for all plugins

    - ``multi_instance`` - `bool`, if True, plugin will be not treated as a singleton
    - ``abstract`` - `bool`, abstract plugins are not registered in :class:`PluginManager`
    - ``platform`` - `list(str)`, platforms where the Plugin can be run
    - ``plugin_id`` - `str`, autoset to lowercase class name
    """

    __metaclass__ = MetaPlugin

    multi_instance = False

    platform = ['any']


    def __new__(cls, *args, **kwargs):
        """ Returns a class instance,
        If it already instantiated, return it
        otherwise return new instance
        """
        if issubclass(cls, PluginManager):
            # If we also a PluginManager, just create and return
            self = super(Plugin, cls).__new__(cls)
            self.plugin_manager = self
            return self

        # Normal case when we are standalone plugin
        self = None
        plugin_manager = args[0]
        if not cls.multi_instance:
            self = plugin_manager.instance_get(cls)

        if self is None:
            self = super(Plugin, cls).__new__(cls)
            self.plugin_manager = plugin_manager
            self.plugin_id = cls.__name__.lower()
            # Allow PluginManager implementation to update Plugin
            plugin_manager.plugin_activated(self)

        return self

    def unload(self):
        """
        Called when plugin class is being unloaded by
        :class:`netcontrol.plugmgr.PluginLoader`
        """
