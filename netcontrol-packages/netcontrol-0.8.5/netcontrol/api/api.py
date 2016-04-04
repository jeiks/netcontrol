from netcontrol.com import Interface

class ICategoryProvider (Interface):
    """
    Base interface for plugins that provide a sidebar entry

    See :class:`netcontrol.api.CategoryPlugin`
    """
    def get_ui(self):
        """
        Should return :class:`netcontrol.ui.Layout` or :class:`netcontrol.ui.Element`
        representing plugin's UI state
        """


class IModuleConfig (Interface):
    """
    Base interface for module configurations.

    See :class:`netcontrol.api.ModuleConfig`
    """

class IEventDispatcher (Interface):
    """
    Base interface for :class:`Plugin` which may dispatch UI Events_.

    See :class:`netcontrol.api.EventProcessor`
    """

    def match_event(self, event):
        pass

    def event(self, event, *params, **kwparams):
        pass


class IXSLTFunctionProvider (Interface):
    """
    Interface for classes which provide additional XSLT functions for
    use in Widgets_ templates.
    """

    def get_funcs(self):
        """
        Gets all XSLT functions provided. Functions are subject to be invoked
        by ``lxml``.

        :returns: dict(str:func)
        """
