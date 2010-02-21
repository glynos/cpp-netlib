#            Copyright (c) Glyn Matthews 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
# This script downloads either a tarball or zipfile of the latest version of the
# main repo.
#


import os
import sys
import urllib2
import urlparse
import optparse
import string
import subprocess


#
# Some constants
#
repo_root = "http://github.com/mikhailberis/cpp-netlib/"
branch = "0.6-devel"
boost_minimum_version = "1_40"


def check_boost_installation():
    """ Gets information about the user's boost environment. """

    env = os.getenv("BOOST_ROOT")
    assert(env is not None)

    # get boost version from version.hpp
    version = None
    version_hpp = os.path.join(env, "boost", "version.hpp")
    f = open(version_hpp, "r")
    for line in f:
        if line.startswith("#define BOOST_LIB_VERSION"):
            begin = string.find(line, '"')
            end = string.rfind(line, '"')
            version = line[begin + 1:end]
            break

    class Params: pass
    params = Params()
    params.root = env
    params.version = version
    return params


def build(bjam, user_config=None):
    """  """

    if bjam is None:
        bjam = "bjam"

    results = open("results.txt", "w+")
    rc = subprocess.call(bjam, stdout=results, stderr=results)
    

def download_tarball(stage):
    """ Downloads the latest tarball from the mikhailberis fork. """
    
    r = urllib2.urlopen(repo_root + "tarball/" + branch)
    filename = urlparse.urlparse(r.geturl()).path.split("/")[-1]
    path = os.path.join(stage, filename)
    f = open(path, "w+")
    f.write(r.read())
    return filename

def unpack_tarball(stage, filename):
    """ Unpacks the tarball into a stage directory. """

    import tarfile
    import shutil

    (root, ext) = os.path.splitext(filename)
    (root, ext) = os.path.splitext(root)
    if os.path.exists(os.path.join(stage, root)):
        shutil.rmtree(os.path.join(stage, root))

    os.chdir(stage)
    f = tarfile.open(os.path.join(stage, filename))
    f.extractall()
    os.chdir(root)
    

def download_zipball(stage):
    """ Downloads the latest zipfile from the mikhailberis fork. """

    r = urllib2.urlopen(repo_root + "zipball/" + branch)
    filename = urlparse.urlparse(r.geturl()).path.split("/")[-1]
    path = os.path.join(stage, filename)
    f = open(path, "w+")
    f.write(r.read())
    return filename

def unpack_zipball(stage, filename):
    """ Unpacks the zip file into a stage directory. """

    import zipfile
    import shutil

    (root, ext) = os.path.splitext(filename)
    (root, ext) = os.path.splitext(root)
    if os.path.exists(os.path.join(stage, root)):
        shutil.rmtree(os.path.join(stage, root))

    os.chdir(stage)
    f = zipfile.ZipFile(os.path.join(stage, filename))
    f.extractall()
    os.chdir(root)


if __name__ == "__main__":
    
    opt = optparse.OptionParser(
        usage="%prog [options]")
    
    opt.add_option("--zip",
                   action="store_false",
                   help="Downloads the zip file.")
    opt.add_option("--tar",
                   action="store_false",
                   help="Downloads the tar.gz file.")
    opt.add_option("--stage",
                   metavar="DIR",
                   help="the stage directory.")
    opt.add_option("--branch",
                   help="the Git branch to check.")
    opt.add_option("--bjam",
                   help="The path to the bjam binary if it's not in the system path.")
    opt.add_option("--user-config",
                   metavar="FILE",
                   help="the user-config file to use.")
    
    (opts, args) = opt.parse_args()

    if (opts.zip is None and opts.tar is None) or \
            (opts.zip is not None and opts.tar is not None):
        print("Please specify either zip or tar")
        sys.exit(-1)

    if opts.stage is None:
        opts.stage = "/tmp"
    print("stage: %s" % opts.stage)

    boost_params = check_boost_installation()
    assert(boost_params.version is not None)
    assert(boost_params.version > boost_minimum_version)
    print("boost_root: %s" % boost_params.root)
    print("boost_version: %s" % boost_params.version)

    try:
        if opts.zip is not None:
            filename = download_zipball(opts.stage)
            unpack_zipball(opts.stage, filename)
        elif opts.tar is not None:
            filename = download_tarball(opts.stage)
            unpack_tarball(opts.stage, filename)

        build(opts.bjam, opts.user_config)

    except Exception, e:
        print(e)
