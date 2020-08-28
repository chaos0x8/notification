#!/usr/bin/ruby

gem 'rake-builder', '~> 2.0', '>= 2.0.9'

require 'rake-builder'

FLAGS = [ "--std=c++17", '-Wall', '-Werror', "-O3", "-s", "-DNDEBUG", '-Wno-deprecated' ]
INCLUDES = [ 'Source', 'cpp-common/Source', 'c8-cpp/src' ]
PKGS = [ 'libnotify', 'gtk+-3.0' ]

C8_CPP = GitSubmodule.new(name: 'c8-cpp', libs: ['lib/libc8-network.a', 'lib/libc8-option-parser.a', 'lib/libc8-common.a'])

INSTALL = InstallPkg.new { |t|
  t.name = :install_pkg
  t.pkgs << 'libnotify-dev'
}

CONFIG = GeneratedFile.new(format: true) { |t|
  max = 60999
  min = 32768
  port = rand(max-min) + min

  t.name = 'Source/Config.cpp'
  t.code = proc { |fn|
    d = []
    d << '#include "Config.hpp"'
    d << ""
    d << "namespace Config"
    d << "{"
    d << "  const std::string PORT = \"#{port}\";"
    d << "}"
  }
}

SHARED_FILES = mkSources(FileList['Source/*.cpp', 'Source/Config.cpp'].uniq, flags: FLAGS, includes: INCLUDES, pkgs: PKGS)

exe = Dir['Source/*'].select { |x| File.directory?(x) }.collect { |dir|
  Executable.new { |t|
    t.name = "bin/#{File.basename(dir)}"
    t.requirements << INSTALL
    t.flags << FLAGS
    t.includes << INCLUDES
    t.libs << [C8_CPP, '-lboost_system']
    t.pkgs << PKGS
    t.sources << SHARED_FILES << FileList["#{dir}/*.cpp"]
  }
}

task(default: Names[exe])
