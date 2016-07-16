Overview
========

In this page we document the process by which the developers of the
project collaborate and get things done. If you're interested in
contributing or getting involved please consider the guidelines and
tips that are outlined in this page. Please check-in often as we flesh
out this document further.

Introduction
============

The first thing contributors and developers have to do is introduce
themselves to the community. We'd like to have all contributors
involved in the decision making process with regards to the
development of both the community and the library. We value
individuals and their personal styles, so the more everyone knows
about everyone the better we can work together to achieve the same
goal.

If you haven't yet, please `subscribe`_ to the developers mailing list
and introduce yourself before proceeding.

.. _`subscribe`: https://groups.google.com/group/cpp-netlib`

Pull Requests
=============

The maintainers of the project review and merge `Pull Requests`_ (from
here on out referred to as PR's) from contributors using the GitHub
Pull Request feature. This allows the project to move forward using
the Git distributed development workflow. If you need an introduction
to git, please refer to the following links for git-specific
information.

.. _`Pull Requests`: https://help.github.com/articles/using-pull-requests

* `ProGit`_ — a website dedicated to basic and advanced git usage.
* `GitHub Git Setup`_ — the GitHub help pages on setting up git to work
  with GitHub.

.. _`ProGit`: http://git-scm.com/book
.. _`GitHub Git Setup`: https://help.github.com/articles/set-up-git

What follows in this section assumes that you're already familiar with
the basic git workflow.

Forking the Repo
================

Forking requires that you already have a GitHub account. Before
continuing, please make sure that you've signed up for a GitHub
account (it's free to develop for open source projects) and have
familiarized yourself with the different development workflows. It's
important that you understand the GitHub workflow before continuing.

The official repository is located in GitHub at
https://github.com/cpp-netlib/cpp-netlib. Before you can submit PR's
you should first create your own fork of the repository on GitHub. You
can fork the repository by clicking on `Fork`_ at the upper right portion
of the page.

.. _`Fork`: https://github.com/cpp-netlib/cpp-netlib/fork

Preparing a PR
==============

Once you have a fork of the repo, determine to which branch you'd like
to send a PR to. In the next section we describe the various branches
we'll be dealing with in the course of development of a release.

When you've determined the branch to which you'd like to send a PR to
you can follow these steps to prepare your change for inclusion in the
library.

1. Create an integration branch. This integration branch should be
   rooted off the branch you intend to send a PR to. For example, if
   you're sending a PR to cpp-netlib/master and your fork is
   user/master, you should create a user/master-integration branch.
2. Create a topic branch. From the integration branch, you can then
   create as many topic branches as you want. It's recommended that you
   isolate all experimentation to branches — once you're resonably sure
   that your work is good to go, merge your topic branch into the
   integration branch in your local repo, then push the changes to your
   GitHub repo.
3. Make sure your integration branch is up to date. To do this you
   should first pull changes to your local master (assuming that's where
   you'd like to send a pull request to), rebase your integration branch
   to the tip of master, then make sure all merge conflicts are dealt
   with. Proceed only when your integration branch is up-to-date with the
   official branch you're going to send your PR to.
4. Send the PR. Once you're reasonably happy with the state of your
   integration branch, send off a PR to the official repo and set the
   destination branch as the branch you intend to send the change to.
5. Address Comments The maintainers will be reviewing your changes, and
   sometimes they may have comments they will ask you to address in
   your PR. You can do this by going back to the second step of this
   process, but you don't need to send another PR -- all you have to do
   is push your changes to your GitHub hosted integration branch and
   your PR will be updated automatically. That said, don't forget to
   update the discussion on the PR that you're ready for the PR to be
   reviewed again.
6. Your PR is merged. If you've done everything correctly up to this
   point, your PR should be cleanly merge-able into the branch you sent
   the PR to. A maintiner will merge you change into the project and
   you're now officially a contributor to the project!


In case you have multiple PR's in flight, you may want to have
multiple integration branches — that is, one integration branch per PR
should be good to keep.

Working Branches
================

The project always has the latest bleeding edge versions of the
library under development in the master branch. This version is
explicitly unstable and subject to (potentially massive) changes over
time.

Once the state of master has stabilized and a release process is
initiated by the project maintainers (it will be announced on the
mailing list) a version-devel branch is started from master and a
release candidate is prepared. For example, if a 1.0 release is
initiated, a branch 1.0-devel is started off master.

A release candidate is tagged off of the version-devel branch on a
regular basis, and is publicized as widely as possible. The tag name
should be of the form version-rcN. Again as an example, the first
release candidate for a 1.0 release will be tagged as 1.0.0-rc0.

All PR's for the upcoming version should go directly to the
version-devel branch.

During the stabilization of the version-devel branch, master remains
open for PR's for new functionality, new accepted libraries, and API
breaking changes.

Once a release candidate is deemed "good to go" by the maintainers we
tag the library (and submodules appropriately) with a tag of the form
version-final. As with earlier examples, the tag for the 1.0 release
would be 1.0.0-final.

Patch Releases
==============

Critical bug fixes go into the version-devel branch after a final
release has been packaged. In case there's a need for update releases,
the release candidate process is followed until another final version
of the patch release is tagged.

In our on-going example, this will be of the form 1.0.1-rc0,
1.0.1-rc1, and so on until it's stabilized — at which time a
1.0.1-final is tagged and packaged.
