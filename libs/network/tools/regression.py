#            Copyright (c) Glyn Matthews 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
# This script downloads either a tarball or zipfile of the latest version of the
# main repo.
#


import os
import urllib2
from urlparse import urlparse
import optparse
import tarfile
import string


repo_root = 'http://github.com/mikhailberis/cpp-netlib/'
stage = '/tmp'
branch = '0.6-devel'



def check_boost_installation():
    """ Gets information about the user's boost environment. """

    env = os.getenv('BOOST_ROOT')
    assert(env is not None)

    # get boost version from version.hpp
    version = None
    version_hpp = os.path.join(env, 'boost', 'version.hpp')
    f = open(version_hpp)
    for line in f:
        if line.startswith('#define BOOST_LIB_VERSION'):
            begin = string.find(line, '"')
            end = string.rfind(line, '"')
            version = line[begin + 1:end]

    assert(version is not None)

    return {'root': env,
            'version': version }
    

def download_tarball():
    """ Downloads the latest tarball from the mikhailberis fork. """

    print repo_root + 'tarball/' + branch
    r = urllib2.urlopen(repo_root + 'tarball/' + branch)
    filename = urlparse(r.geturl()).path.split('/')[-1]
    path = os.path.join(stage, filename)
    f = open(path, 'w+')
    f.write(r.read())
    return filename

def unpack_tarball(stage, filename):
    """ Unpacks the tarball into a stage directory. """

    os.chdir(stage)
    f = tarfile.open(os.path.join(stage, filename))
    f.extractall()
    (root, ext) = os.path.splitext(filename)
    print(root)
    os.chdir(root)
    

def download_zipball():
    """ Downloads the latest zipfile from the mikhailberis fork. """

    r = urllib2.urlopen(repo_root + '/zipball/' + branch)
    filename = urlparse(r.geturl()).path.split('/')[-1]
    path = os.path.join(stage, filename)
    f = open(path, 'w+')
    f.write(r.read())
    return filename

def unpack_zipball(stage, filename):
    """ Unpacks the zip file into a stage directory. """

    f = zipfile.ZipFile(os.path.join(stage, filename))
    f.extractall()


def build():
    """ Invokes bjam.  Runs all unit tests. """

    # Somehow we have to report any failures


if __name__ == '__main__':

    params = check_boost_installation()
    print(params)
    
    # try:
    #     opt = optparse.OptionParser(
    #         usage="%prog [options]")
    # 
    #     opt.add_option('--zip',
    #                    help='uses the zip')
    #     opt.add_option('--tar',
    #                    help='uses the tar.gz')
    #     opt.add_option('--stage',
    #                    help='the stage directory')
    #     opt.add_option('--branch',
    #                    help='the branch to check.')
    # 
    #     opt.parse_args()
    #     
    #     
    #     filename = download_tarball()
    #     print(filename)
    #     unpack_tarball(stage, filename)
    #     
    # except urllib2.URLError, e:
    #     print('%s' % e)
