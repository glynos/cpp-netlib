#            Copyright (c) Glyn Matthews 2010.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
# This script downloads either a tarball or zipfile of the latest version of the
# main repo, unpacks it, runs bjam (including all unit tests) and creates a file
# that contains all the information needed to evaluate any regression failures.
#
# 1. Create a directory where you want to run the test
# 2. Download this script
# 3. Run this script: python regression.py [options]
# 4. E-mail the `cpp_netlib_regression.txt` file to the developers
#


import os
import sys
import urllib2
import urlparse
import optparse
import string
import subprocess



class RegressionTester:
    """  """

    def __init__(self):
        """  """

        self._repo_root = "http://github.com/mikhailberis/cpp-netlib/"
        self._boost_minimum_version = "1_40"

    def check_boost_installation(self):
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

    def download(self, branch):
        """  """

        raise NotImplementedError

    def get_commit(self, filename):
        """  """

        (root, ext) = os.path.splitext(filename)
        (root, ext) = os.path.splitext(root)
        return root.split("-")[-1]
    
    def unpack(self, filename):
        """  """

        raise NotImplementedError

    def build(self, results, bjam):
        """  """

        rc = subprocess.call(bjam, stdout=results, stderr=results)

    def run(self, runner, branch, bjam=None, user_config=None):
        """  """
        
        boost_params = self.check_boost_installation()
        assert(boost_params.version is not None)
        assert(boost_params.version > self._boost_minimum_version)

        results = "cpp_netlib_regression.txt"
        results_file = open(results, "w+")
        
        filename = self.download(branch)
        commit = self.get_commit(filename)
        
        self.unpack(filename)

        if bjam is None:
            bjam = "bjam"
    
        if user_config is not None:
            bjam = "%s --user-config=%s" % (bjam,u ser_config)
        
        self.build(results_file, bjam)

        results_file.write("\n\n")
        results_file.write("runner: %s\n" % runner)
        results_file.write("git_branch: %s\n" % branch)
        results_file.write("boost_root: %s\n" % boost_params.root)
        results_file.write("boost_version: %s\n" % boost_params.version)
        results_file.write("commit: %s\n" % commit)

        return results


class RegressionTesterWithTar(RegressionTester):
    """  """

    def __init__(self):
        RegressionTester.__init__(self)

    def download(self, branch):
        """ Downloads the latest tarball from the mikhailberis fork. """
    
        r = urllib2.urlopen(self._repo_root + "tarball/" + branch)
        filename = urlparse.urlparse(r.geturl()).path.split("/")[-1]
        f = open(filename, "w+")
        f.write(r.read())
        return filename

    def unpack(self, filename):
        """ Unpacks the tarball into a stage directory. """

        import tarfile
        import shutil

        (root, ext) = os.path.splitext(filename)
        (root, ext) = os.path.splitext(root)
        if os.path.exists(root):
            shutil.rmtree(root)

        f = tarfile.open(filename)
        f.extractall()
        os.chdir(root)


class RegressionTesterWithZip(RegressionTester):
    """  """

    def __init__(self):
        RegressionTester.__init__(self)
        
    def download(self, branch):
        """ Downloads the latest zipfile from the mikhailberis fork. """

        r = urllib2.urlopen(self._repo_root + "zipball/" + branch)
        filename = urlparse.urlparse(r.geturl()).path.split("/")[-1]
        f = open(filename, "w+")
        f.write(r.read())
        return filename

    def unpack(self, filename):
        """ Unpacks the zip file into a stage directory. """

        import zipfile
        import shutil

        (root, ext) = os.path.splitext(filename)
        (root, ext) = os.path.splitext(root)
        if os.path.exists(root):
            shutil.rmtree(root)

        f = zipfile.ZipFile(filename)
        f.extractall()
        os.chdir(root)


if __name__ == "__main__":
    
    opt = optparse.OptionParser(
        usage="%prog [options]")

    opt.add_option("--runner",
                   metavar="RUNNER",
                   help="A real name to identify the test runner.")
    opt.add_option("--package",
                   metavar="ZIP/TAR",
                   help="Specify 'zip' to download the zip file, or 'tar' to "
                        "download the tarball.")
    opt.add_option("--branch",
                   help="the Git branch to check.")
    opt.add_option("--bjam",
                   help="The path to the bjam binary if it's not in the system "
                        "path.")
    opt.add_option("--user-config",
                   metavar="FILE",
                   help="the user-config file to use.")
    
    (opts, args) = opt.parse_args()

    if opt.runner is None:
        opt.print_help()
        sys.exit(-1)

    if opts.branch is None:
        opt.print_help()
        sys.exit(-1)

    if opts.package not in ["zip", "tar"]:
        opt.print_help()
        sys.exit(-1)
        
    if opts.package == "zip":
        tester = RegressionTesterWithZip()
    elif opts.package == "tar":
        tester = RegressionTesterWithTar()

    try:
        results = tester.run(opts.runner,
                             opts.branch,
                             bjam=opts.bjam,
                             user_config=opts.user_config)
        print("The regression results are found in `%s`." % results)
        print("Please e-mail this to the project administrators at `cpp.netlib@gmail.com`")
    except Exception, e:
        print(e)
