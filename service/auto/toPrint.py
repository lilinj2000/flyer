#!/usr/bin/python

import re

struct_pattern = re.compile(r'''struct (.*)''')

left_brace_pattern = re.compile(r'''\{''')

right_brace_pattern = re.compile(r'''\};''')

comment_pattern = re.compile(r'''//''')

field_pattern = re.compile(r'''\s+(\w+)\s+(\w+);''')

file = open("SgitFtdcUserApiStruct.h")

struct_status = False
struct_name = ''
filed_name = ''
object_name = ''
tab = '    '
first_field = False

header_file = open('SgitFtdcUserApiStructPrint.hh', 'wb')
# cpp_file = open('FtdcUserApiStructPrint.cc', 'wb')

header_file.write('#ifndef SGITFTDC_USERAPI_STRUCT_PRINT_HH\n')
header_file.write('#define SGITFTDC_USERAPI_STRUCT_PRINT_HH\n\n')

header_file.write('#include <ostream>\n')
header_file.write('#include "SgitFtdcUserApiStruct.h"\n\n')

# header_file.write('namespace ctp\n')
# header_file.write('{\n')

# cpp_file.write('#include "FtdcUserApiStructPrint.hh"\n\n')
# cpp_file.write('namespace ctp\n')
# cpp_file.write('{\n')

for line in file:
    # print line
    result = struct_pattern.search(line)
    if result:
        # print '%s' % result.groups()
        struct_name = result.group(1)
        object_name = 'a' + struct_name.replace('CSgitFtdc', '')
        # header_file.write('std::ostream& operator<<(std::ostream&, const %s&);\n\n' % struct_name)
        header_file.write('template< typename CharT, typename TraitsT >\n')
        header_file.write('std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, %s const& %s)\n' % (struct_name, object_name))
        # print '%s' % object_name
        # cpp_file.write('std::ostream& operator<<(std::ostream& os, const %s& %s)\n' % (struct_name, object_name))
        continue

    result = left_brace_pattern.search(line)
    if result:
        # print 'left brace: %s' % line
        struct_status = True
        header_file.write('{\n')
        header_file.write('    os <<std::endl;\n')
        header_file.write('    os <<"{" <<std::endl;\n')
        header_file.write(r'    os <<"    \"%s\": {" <<std::endl;' %  struct_name)
        header_file.write('\n')

        # cpp_file.write('{\n')
        # cpp_file.write('    os <<"{" <<std::endl;\n')
        # cpp_file.write(r'    os <<"    \"%s\": {" <<std::endl;' %  struct_name)
        # cpp_file.write('\n')
        first_field = True
        continue

    result = right_brace_pattern.search(line)
    if result:
        # print 'right brace: %s' % line
        struct_status = False
        header_file.write(r' <<"\"" <<std::endl;')
        header_file.write('\n')
        header_file.write('    os <<"    }" <<std::endl;\n')
        header_file.write('    os <<"}" <<std::endl;\n')
        header_file.write('}\n\n')

        # cpp_file.write(r' <<"\"" <<std::endl;')
        # cpp_file.write('\n')
        # cpp_file.write('    os <<"    }" <<std::endl;\n')
        # cpp_file.write('    os <<"}" <<std::endl;\n')
        # cpp_file.write('}\n\n')
        continue

    result = comment_pattern.search(line)
    if result:
        # print 'comment: %s' % line
        continue

    result = field_pattern.search(line)
    if result:
        # print 'field: %s\t%s' % (result.group(1), result.group(2))
        if struct_status:
            # print 'field: %s.%s' % (struct_name, result.group(2))
            # fieldname: obj.filed_value
            if not first_field:
                header_file.write(r' <<"\"," <<std::endl;')
                header_file.write('\n')

                # cpp_file.write(r' <<"\"," <<std::endl;')
                # cpp_file.write('\n')

            # cpp_file.write(r'    os <<"        \"%s\": \"" <<%s.%s ' % (result.group(2), object_name, result.group(2)))
            header_file.write(r'    os <<"        \"%s\": \"" <<%s.%s ' % (result.group(2), object_name, result.group(2)))
            first_field = False
        continue

    # print line


header_file.write('#endif // SGITFTDC_USERAPI_STRUCT_PRINT_HH\n')

    


