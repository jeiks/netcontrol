import subprocess

def version():
    """ Returns version (including git revision
    if netcontrol runs inside a working tree
    :returns: str
    """

    release = '0.6.1'
    p = subprocess.Popen('git describe --tags 2> /dev/null',
            shell=True,
            stdout=subprocess.PIPE)
    if p.wait() != 0:
        return release
    return p.stdout.read().strip('\n ')

# Generation declares which plugin version range to use
generation = 1
