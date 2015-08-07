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

def web_require url
    system "wget #{url}" unless File.exist?(File.basename(url))
    require_relative File.basename(url)
end

web_require "https://raw.github.com/chaos0x8/rake-builder/master/RakeBuilder.rb"

if ARGV.index("nodebug").nil?
    FLAGS = [ "--std=c++1y", "-g" ]
else
    FLAGS = [ "--std=c++1y", "-O3", "-s", "-DNDEBUG" ]

    task :nodebug
end

Application.new do |t|
    t.name = 'bin/notification'
    t.flags = FLAGS
    t.files = FileList['Source/*.cpp']
    t.libs = [ Pkg.new('libnotify'), Pkg.new('gtk+-2.0'), '-LcppCommon/lib', '-lcommon' ]
    t.includes = [ 'Source', 'cppCommon/Source' ]
    t.dependencies = [ 'cppCommon/lib/libcommon.a' ]
end

file 'cppCommon/.git' do
    sh 'git submodule init'
    sh 'git submodule update'
end

file 'cppCommon/lib/libcommon.a' => [ 'cppCommon/.git', :rebuild ] do
    sh 'cd cppCommon && rake lib/libcommon.a'
end

task :default => [ 'bin/notification' ]
