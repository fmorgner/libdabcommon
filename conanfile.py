# pylint: disable=missing-docstring
from conans import ConanFile


class LibDABCommonConan(ConanFile):
    name = 'libdabcommon'
    version = '1.0.1'
    description = (
        'The common subsystem for the ODR DAB data toolkit, that provides '
        'types, functions, and constants used throughout the ODR DAB data '
        'toolkit'
    )
    settings = None
    options = {
        "test": [True, False],
    }
    default_options = (
        'test=True',
    )
    url = 'https://github.com/Opendigitalradio/libdabcommon.git'
    license = 'BSD 3-clause'
    exports_sources = (
        '**/CMakeLists.txt',
        '**/*.h',
        '**/*.cpp',
        '**/*.cmake',
    )

    def build(self):
        dabdecode_test = '-DDABCOMMON_ENABLE_TESTS=%s' % (
            'On' if self.options.test
            else 'Off'
        )

        dabdecode_prefix = '-DCMAKE_INSTALL_PREFIX="%s"' % (
            self.package_folder
        )

        self.run(('cmake {directory}'
                  ' {prefix}'
                  ' {test}').format(**{
                      'directory': self.conanfile_directory,
                      'prefix': dabdecode_prefix,
                      'test': dabdecode_test,
                  }))

        self.run('cmake --build . --target install')

    def package_info(self):
        self.cpp_info.libs = [
            'pthread'
        ]
        self.cpp_info.includedirs = [
            'include'
        ]

    def requirements(self):
        if self.options.test:
            self.requires('CUTEX/[>=1.0]@fmorgner/stable')
