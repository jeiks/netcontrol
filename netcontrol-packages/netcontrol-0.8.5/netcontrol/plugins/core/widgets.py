from netcontrol.ui import *
from netcontrol.com import implements, Plugin
from netcontrol.api import *
from netcontrol import apis
from updater import Updater

# We want apis.dashboard already!
import netcontrol.plugins.dashboard.api


class NewsWidget(Plugin):
    implements(apis.dashboard.IWidget)
    title = 'Project news'
    icon = '/dl/core/ui/stock/news.png'
    name = 'Project news'
    style = 'normal'

    def get_ui(self, cfg, id=None):
        ui = self.app.inflate('core:news')
        feed = Updater.get().get_feed()
        if feed is not None:
            for i in feed[:3]:
                ui.append('list', UI.CustomHTML(html='<li>%s</li>'%i['text']))
        return ui

    def handle(self, event, params, cfg, vars=None):
        pass

    def get_config_dialog(self):
        return None

    def process_config(self, event, params, vars):
        pass
