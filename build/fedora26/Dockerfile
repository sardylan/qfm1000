FROM fedora:26

RUN yum -y update \
    && yum install -y qt5-devel \
    && yum groupinstall "Development Tools" \
    && yum install gcc-c++
