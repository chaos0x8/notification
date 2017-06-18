#!/usr/bin/ruby

# \author <https://github.com/chaos0x8>
# \copyright
# Copyright (c) 2015, <https://github.com/chaos0x8>
#
# \copyright
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# \copyright
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

gem 'rake-builder', '~> 0.4.0'

require 'RakeBuilder'

FLAGS = [ "--std=c++17", '-Wall', '-Werror', "-O3", "-s", "-DNDEBUG", '-Wno-deprecated' ]
INCLUDES = [ 'Source', 'cppCommon/Source' ]
PKGS = [ 'libnotify', 'gtk+-3.0' ]

CPP_COMMON = GitSubmodule.new(name: 'cppCommon', libs: ['lib/libcommonOptionParser.a', 'lib/libcommonNetwork.a', 'lib/libcommon.a'])

CONFIG = GeneratedFile.new(name: 'Source/Config.cpp') { |t|
  max = 60999
  min = 32768
  port = rand(max-min) + min

  t.code = proc { |fn|
    File.open(fn, 'w') { |f|
      f.write '#include "Config.hpp"' "\n"
      f.write "\n"
      f.write "namespace Config\n"
      f.write "{\n"
      f.write "  const std::string PORT = \"#{port}\";\n"
      f.write "}\n"
    }
  }
}

SHARED_FILES = mkSources(FileList['Source/*.cpp', 'Source/Config.cpp'].uniq, flags: FLAGS, includes: INCLUDES, pkgs: PKGS)

exe = Dir['Source/*'].select { |x| File.directory?(x) }.collect { |dir|
  Executable.new { |t|
    t.name = "bin/#{File.basename(dir)}"
    t.flags << FLAGS
    t.includes << INCLUDES
    t.libs << CPP_COMMON << ['-lboost_system']
    t.pkgs << PKGS
    t.sources << SHARED_FILES << FileList["#{dir}/*.cpp"]
  }
}

task(default: Names[exe])
