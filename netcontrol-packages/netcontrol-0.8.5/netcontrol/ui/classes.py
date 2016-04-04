import random
from lxml import etree
from netcontrol.utils import fix_unicode


class Element(etree.ElementBase):
    """
    XML layout element - derived from lxml.ElementBase.
    See http://lxml.de/api/lxml.etree.ElementBase-class.html

    `args` should be child elements and `kwargs` - attributes.
    """
    def __init__(self, tag, *args, **kwargs):
        etree.ElementBase.__init__(self)
        self.tag = tag.lower()
        if not 'id' in kwargs.keys() or kwargs['id'] is None:
            self['id'] = str(random.randint(1,9000*9000))
        self._init(*args, **kwargs)
        self._children = []
        for k in args:
            self.append(k)
        for k in kwargs:
            if kwargs[k] is not None:
                self[k] = kwargs[k]

    def _init(self, *args, **kwargs):
        etree.ElementBase._init(self)
        if not hasattr(self, '_children'):
            self._children = []

    def append(self, el):
        """
        Appends an :class:`Element` or :class:`Layout` to current element.
        """
        if el is not None:
            if hasattr(el, 'elements'):
                el = el.elements()
            self._children.append(el)
            etree.ElementBase.append(self, el)
        return self

    def append_all(self, *els):
        """
        Appends all :class:`Element` or :class:`Layout` instances to current element.
        """
        for el in els:
            self.append(el)
        return self

    def __setitem__(self, idx, val):
        self.set(idx, val)

    def set(self, attr, val):
        """
        Sets `attr` attribute to `val`, converting value to unicode string.
        """
        etree.ElementBase.set(self, attr, fix_unicode(str(val)))
        return self

    def __getitem__(self, idx):
        return self.get(idx)


class UI(object):

    """
    Automatically generate XML tags by calling name

    >>> m = UI.Meta(encoding="ru")
    >>> m.toxml()
    '<meta encoding="ru"/>'
    >>>

    Some of tags have overriding classes here.
    """

    __overrides_cache = None

    class __metaclass__(type):
        def __getattr__(cls, name):
            return lambda *args, **kw: Element(name.lower(), *args, **kw)

    @staticmethod
    def list_overrides():
        if UI.__overrides_cache is None:
            UI.__overrides_cache = dict(
                [(x.lower(),getattr(UI,x)) for x in UI.__dict__]
            )
        return UI.__overrides_cache

    @staticmethod
    def gen(name, *args, **kwargs):
        """ Generate XML tags by name, if name will violate Pyhton syntax

        >>> xi = UI.gen('xml:include', href="some.xml")
        >>> xi.toxml()
        '<xml:include href="some.xml"/>'
        >>>
        """
        return Element(name.lower(), *args, **kwargs)

    class ProgressBar(Element):
        def __init__(self, value=0, max=1, width=1):
            Element.__init__(self, 'progressbar')
            self['right'] = width - int(value*width/max)
            self['left'] = int(value*width/max)

    class LT(Element):
        def __init__(self, *args, **kwargs):
            Element.__init__(self, 'lt', **kwargs)
            for e in args:
                if isinstance(e, Element):
                    if e.tag == 'ltr':
                        self.append(e)
                    else:
                        c = UI.LTR(e)
                        c['spacing'] = self['spacing']
                        self.append(c)

    class LTR(Element):
        def __init__(self, *args, **kwargs):
            Element.__init__(self, 'ltr', **kwargs)
            for e in args:
                if isinstance(e, Element):
                    if e.tag == 'ltd':
                        self.append(e)
                    else:
                        c = UI.LTD(e)
                        c['spacing'] = self['spacing']
                        self.append(c)

    class DT(Element):
        def __init__(self, *args, **kwargs):
            Element.__init__(self, 'dt', **kwargs)
            for e in args:
                if isinstance(e, Element):
                    if e.tag == 'dtr':
                        self.append(e)
                    else:
                        self.append(UI.DTD(e))
            for e in args:
                self.append(e)

    class DTR(Element):
        def __init__(self, *args, **kwargs):
            Element.__init__(self, 'dtr', **kwargs)
            for e in args:
                if isinstance(e, Element):
                    if e.tag in ['dtd', 'dth', 'statuscell']:
                        self.append(e)
                    else:
                        self.append(UI.DTD(e))

    class TreeContainer(Element):
        def __init__(self, *args, **kwargs):
            Element.__init__(self, 'treecontainer', **kwargs)
            for e in args:
                if isinstance(e, Element):
                    if e.tag == 'treecontainer':
                        self.append(e)
                    elif e.tag == 'treecontainernode':
                        self.append(e)
                    else:
                        self.append(UI.TreeContainerNode(e))

    class TabControl(Element):
        def __init__(self, *args, **kwargs):
            Element.__init__(self, 'tabcontrol', **kwargs)
            self.tc = 0

        def add(self, name, content, form=None, id=None):
            if id is None:
                id = str(self.tc)
            tb = UI.TabBody(content, id=id)
            self.append(UI.TabHeader(text=name, id=(id or tb['id']), live=(content is None), form=form))
            if content is not None:
                self.append(tb)
            self.tc += 1



class TreeManager(object):
    """
    Processes treenode click events and stores the nodes' collapsed/expanded
    states. You should keep the TreeManager inside the session, call
    node_click() on each 'click' event, and apply() to the tree object before
    rendering it.
    """
    states = None

    def __init__(self):
        self.reset()

    def reset(self):
        """
        Removes all saved node states.
        """
        self.states = []

    def node_click(self, id):
        """
        Toggles node state (collapsed/expanded)
        """
        if id in self.states:
            self.states.remove(id)
        else:
            self.states.append(id)

    def apply(self, tree):
        """
        Applies saved node states to a TreeContainer element
        """
        try:
            tree['expanded'] = tree['id'] in self.states

            for n in tree._children:
                if n.tag == 'treecontainer':
                    self.apply(n)
        except:
            raise
