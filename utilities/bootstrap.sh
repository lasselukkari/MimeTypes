#!/bin/bash

set -x

vercmp() {
    if [[ $1 == $2 ]]; then
        return 0
    fi
    local IFS=.
    local i ver1=($1) ver2=($2)
    # fill empty fields in ver1 with zeros
    for ((i = ${#ver1[@]}; i < ${#ver2[@]}; i++)); do
        ver1[i]=0
    done
    for ((i = 0; i < ${#ver1[@]}; i++)); do
        if [[ -z ${ver2[i]} ]]; then
            # fill empty fields in ver2 with zeros
            ver2[i]=0
        fi
        if ((10#${ver1[i]} > 10#${ver2[i]})); then
            return 1
        fi
        if ((10#${ver1[i]} < 10#${ver2[i]})); then
            return 2
        fi
    done
    return 0
}

retry() {
    local -r -i max_attempts="$1"; shift
    local -i attempt_num=1
    until "$@"
    do
        if ((attempt_num==max_attempts))
        then
            echo "Attempt $attempt_num failed and there are no more attempts left!"
            return 1
        else
            echo "Attempt $attempt_num failed! Trying again in $attempt_num seconds..."
            sleep $((attempt_num++))
        fi
    done
}

unameOut="$(uname -s)"
arch="x64"
case "${unameOut}" in
    Linux*)
        machine=linux
        apt clean
        apt update
        retry 10 apt install -y apt-transport-https ca-certificates \
            git build-essential ccache ninja-build pkg-config \
            python3-pip python3-all-dev \
            libicu-dev

        update-ca-certificates -f

        boost_version="$(apt-cache madison libboost-all-dev | grep -oP "\d+(\.\d+)+")"
        vercmp $boost_version "1.66"

        if [[ $? -ne 2 ]]; then
            retry 10 apt install -y libboost-all-dev
        else
            retry 10 apt install -y software-properties-common
            add-apt-repository ppa:mhier/libboost-latest -y
            apt update
            retry 10 apt install -y libboost1.68-dev
        fi

        hash cmake 2>/dev/null || { pip3 install -i https://mirrors.aliyun.com/pypi/simple cmake; }
        ;;
    Darwin*)
        machine=osx
        hash sudo 2>/dev/null || { echo >&2 "Plesse ensure you have executed 'xcode-select --install' to have build tools ready."; exit; }
        brew install boost@1.67 icu4c
        ;;
    CYGWIN*)        machine=win;;
    MINGW32_NT)
        machine=win
        arch=x86
        ;;
    MINGW64_NT)
        machine=win
        ;;
    *)
        machine="UNKNOWN:${unameOut}"
        echo "Not supported platform: ${machine}"
        exit -1
esac