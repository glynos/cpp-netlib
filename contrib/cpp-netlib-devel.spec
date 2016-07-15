Name:        cpp-netlib-devel
Version:     0.9.4
Release:     1%{?dist}
Summary:     The C++ Network Library Project
License:     Boost
URL:         http://cpp-netlib.org/
Source:      https://github.com/downloads/cpp-netlib/cpp-netlib/cpp-netlib-%{version}.tar.gz
BuildArch:   noarch

%description
The project aims to build upon the latest C++ standard (currently
C++11) to provide easy to use libraries for network programming. We use
the latest compiler versions and features with an eye on pushing the
boundaries on leveraging what's available in C++.

Currently the library contains an HTTP client and server implementation,
a stand-alone URI library, a network message framework, and some
concurrency tools.

%prep
%setup -q -n cpp-netlib-%{version}

%build

# The source file will contain the header files needed, so there is no need to build anything

%install
mkdir -p %{buildroot}%{_includedir}
tar cf - boost | (cd %{buildroot}%{_includedir}; tar xf -)

%files
%doc RATIONALE.txt README.rst
%license LICENSE_1_0.txt
%{_includedir}/boost

%changelog
* Fri Jul 15 2016 Håkon Løvdal <hlovdal@gmail.com> - 0.9.4-1
- Created
