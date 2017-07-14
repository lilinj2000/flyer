#!/usr/bin/python

import re

struct_pattern = re.compile(r'''struct (.*)''')

left_brace_pattern = re.compile(r'''\{''')

right_brace_pattern = re.compile(r'''\};''')

comment_pattern = re.compile(r'''//''')

field_pattern = re.compile(r'''\s+(\w+)\s+(\w+);''')

file = open("ThostFtdcUserApiStruct.h")

struct_status = False
struct_name = ''
class_name = ''
object_name = ''
member_name = ''
para_name = ''
field_name = ''
tab = '    '
first_field = False

message_file = open('Message.hh', 'wb')
# message_cpp_file = open('Message.cc', 'wc')
field_file = open('FieldMessage.hh', 'wb')

message_file.write('#ifndef CTP_MESSAGE_HH_\n')
message_file.write('#define CTP_MESSAGE_HH_\n\n')
message_file.write('namespace ctp\n')
message_file.write('{\n\n')

message_file.write('class Message\n')
message_file.write('{\n')
message_file.write('  public:\n')
message_file.write('    typedef enum\n')
message_file.write('    {\n')


field_file.write('#ifndef CTP_FIELD_MESSAGE_HH_\n')
field_file.write('#define CTP_FIELD_MESSAGE_HH_\n\n')
field_file.write('#include "Message.hh"\n')
field_file.write('#include "ThostFtdcUserApiStruct.h"\n')
field_file.write('#include "FtdcUserApiStructPrint.hh"\n\n')
field_file.write('#include <boost/shared_array.hpp>\n')
field_file.write('#include <ostream>\n')
field_file.write('#include <cstring>\n')
field_file.write('#include <stdexcept>\n\n')
field_file.write('namespace ctp\n')
field_file.write('{\n\n')

field_file.write('class FieldMessage : public Message\n')
field_file.write('{\n')
field_file.write('  public:\n')
field_file.write('    virtual Message* toReturnMessage() = 0;\n\n')
field_file.write('    virtual Message* toResponseMessage() = 0;\n\n')
field_file.write('    virtual ~FieldMessage() {};\n\n')
field_file.write('};\n\n')



# message_cpp_file.write('#include "Message.hh"\n')
# message_cpp_file.write('#include "FieldMessage.hh"\n')
# message_cpp_file.write('#include <stdexcept>\n')
# message_cpp_file.write('#include <memory>\n')
# message_cpp_file.write('namespace ctp\n')
# message_cpp_file.write('{\n\n')
# message_cpp_file.write('Message* Message::create(MessageType field)\n')
# message_cpp_file.write('{\n\n')
# message_cpp_file.write('  std::auto_ptr<Message> message;\n\n')
# message_cpp_file.write('  switch(field)\n')
# message_cpp_file.write('  {\n')


for line in file:
    # print line
    result = struct_pattern.search(line)
    if result:
        # print '%s' % result.groups()
        struct_name = result.group(1)
        class_name = struct_name.replace('CThostFtdc', '') + 'Message'
        field_name = class_name.upper()
        member_name = class_name.lower() + '_'
        para_name = class_name.lower()
        object_name = 'a' + class_name
        # print '%s - %s - %s - %s - %s' % (struct_name, class_name, field_name, member_name, object_name)
        field_file.write('class %s : public FieldMessage\n' % class_name)

        message_file.write('      %s,\n' % field_name)

        # message_cpp_file.write('    case %s:\n' % field_name)
        # message_cpp_file.write('      message.reset(new %s());\n' % class_name)
        # message_cpp_file.write('      break;\n\n')
        continue

    result = left_brace_pattern.search(line)
    if result:
        # print 'left brace: %s' % line
        struct_status = True
        field_file.write('{\n')
        field_file.write('  public:\n')
        # construct function
        field_file.write('    %s(const %s* %s):\n' % (class_name, struct_name, para_name))
        field_file.write('      type_(%s)\n' % field_name)
        field_file.write('    {\n')
        field_file.write('      memset(&%s, 0x0, sizeof(%s));\n\n' % (member_name, member_name))
        field_file.write('      if( %s )\n' % para_name)
        field_file.write('      {\n')
        field_file.write('        memcpy(&%s, %s, sizeof(%s));\n' % (member_name, para_name, member_name))
        field_file.write('      }\n')
        field_file.write('    }\n\n')
        # destroy function
        field_file.write('    virtual ~%s() {}\n\n' % class_name)

        # inherit from Message
        field_file.write('    virtual MessageType type() const\n')
        field_file.write('    {\n')
        field_file.write('      return type_;\n')
        field_file.write('    }\n\n')

        field_file.write('    virtual Message* toReturnMessage()\n')
        field_file.write('    {\n')
        field_file.write('      throw std::runtime_error("the type %s not support this function");\n' % class_name)
        field_file.write('    }\n\n')
        field_file.write('    virtual Message* toResponseMessage()\n')
        field_file.write('    {\n')
        field_file.write('      throw std::runtime_error("the type %s not support this function");\n' % class_name)
        field_file.write('    }\n\n')


        # member
        field_file.write('    %s %s;\n\n' % (struct_name, member_name))
        field_file.write('  private:\n\n')
        field_file.write('    MessageType type_;\n')

        first_field = True
        continue

    result = right_brace_pattern.search(line)
    if result:
        # print 'right brace: %s' % line
        struct_status = False
        field_file.write('};\n\n')
        field_file.write('template< typename CharT, typename TraitsT >\n')
        field_file.write('std::basic_ostream< CharT, TraitsT >& operator<<(std::basic_ostream< CharT, TraitsT >& os, %s const& %s)\n' % (class_name, object_name))
        field_file.write('{\n')
        field_file.write('    os <<std::endl;\n')
        field_file.write('    os <<"## %s ##" <<std::endl;\n' % class_name)
        field_file.write('    os <<%s.%s; \n' % (object_name, member_name))
        field_file.write('    os <<"## end %s ##" <<std::endl;\n' % class_name)
        field_file.write('}\n\n')

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
                pass

            first_field = False
        continue

    # print line


with open("field_extend") as msg:
    for line in msg:
        field_file.write('%s' % line)
field_file.write('}; // namespace ctp\n\n')
field_file.write('#endif // CTP_FIELD_MESSAGE_HH_\n')

with open("message_type") as msg:
    for line in msg:
        message_file.write('      %s' % line)

message_file.write('    }MessageType;\n\n')
message_file.write('    virtual MessageType type() const = 0;\n\n')
message_file.write('    virtual ~Message() {};\n\n')
message_file.write('};\n\n')
message_file.write('}; // namespace ctp\n\n')
message_file.write('#endif // CTP_MESSAGE_HH_\n')

    
# message_cpp_file.write('    default:\n')
# message_cpp_file.write('      throw std::runtime_error("the type not supported");\n')
# message_cpp_file.write('  }\n\n')
# message_cpp_file.write('  return message.release();\n\n')
# message_cpp_file.write('}\n\n')
# message_cpp_file.write('}; // namespace ctp\n')

