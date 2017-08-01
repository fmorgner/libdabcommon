# pylint: disable=missing-docstring
from conans import ConanFile, CMake


class LibdabcommonConan(ConanFile):
    name = 'libdabcommon'
    description = """The common subsystem for the ODR DAB data toolkit"""
    license = 'BSD 3-clause'
    version = '1.0.1'
    url = 'https://github.com/fmorgner/libdabcommon.git'
    settings = None
    options = {"test": [True, False]}
    default_options = "test=False"

    def build(self):
        if self.options.test:
            cmake = CMake(self)
            self.run('cmake libdabcommon %s' % cmake.command_line)
            self.run('cmake --build . %s' % cmake.build_config)

    def package(self):
        self.copy('*.h', dst='include', src='libdabcommon/include')

    def package_info(self):
        self.cpp_info.libs = ['pthread']

    def requirements(self):
        if self.options.test:
            self.requires('CUTEX/[>=1.0]@fmorgner/stable')

    def source(self):
        self.run('git clone %s' % self.url)
