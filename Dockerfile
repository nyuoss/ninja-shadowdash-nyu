# Use a base image
FROM cimg/base:stable

USER root

# Install clang-tidy and python3
RUN apt-get update && \
    apt-get install -y clang-tidy clang-format python3 && \
    apt-get clean && rm -rf /var/lib/apt/lists/*
RUN [ -x "$(command -v python)" ] || ln -s $(command -v python3) /usr/bin/python