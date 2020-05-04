import os
from abc import ABC, abstractmethod
class PreProcessorSourceObject:
    def __init__(self, source_in):
        self.source = source_in
        self.full_file_name_no_ext, self.extension = os.path.splitext(source_in)
        self.basename = os.path.basename(source_in)
        self.build_error = "success"
        self.error_line = -1
        self.has_any_preprocessor_usage = False
        self.lines = []
        self.hwpp_pragma_invocation_types = ["SerialIoStructStart", "SerialIoMember", "SerialIoStructEnd", "include"]
        self.is_hwpp_dependency = False
        with open(source_in) as fp:
            while True:
                line = fp.readline()
                self.lines.append(line)
                if not line:
                    break
        self.get_is_include()

    def analyze(self):
        if not (self.extension in [".cu", ".cpp", ".h"]):
            self.build_error = "unrecognized extension \"{}\"".format(self.extension)
            return False
        return True

    def generate_code_lines(self):
        output = []
        output.append("")
        output.append("//Generated from {}".format(self.source))
        output.append("//Begin {}".format(self.basename))
        output.append("")
        macros = self.make_macros()
        for macro in macros:
            macro_lines = macro.get_code_lines()
            for line in macro_lines:
                output.append(line)
        output.append("")
        output.append("//End {}".format(self.basename))
        output.append("")
        return output

    def get_is_include(self):
        for i in range(len(self.lines)):
            line = self.lines[i]
            if self.is_hwpp_pragma_invocation_type(line):
                pragmatype = self.get_pragma_type(line)
                if pragmatype == "include":
                    self.is_hwpp_dependency = True

    def make_macros(self):
        macros = []
        lines = self.lines
        for i in range(len(lines)):
            line = lines[i]
            if self.is_hwpp_pragma_invocation_type(line):
                pragmatype = self.get_pragma_type(line)
                if pragmatype == "include":
                    self.is_hwpp_dependency = True
                if self.get_closing_pragma(pragmatype):
                    j = self.check_closure(i, lines, pragmatype)
                    if j > 0:
                        macros.append(self.process_closed_pragma(i, j, lines, pragmatype))
        return macros

    def get_pragma_arg(self, line, pragmatype, argnum):
        ar = line.strip().split(" ")
        output = ar[ar.index(pragmatype)+argnum+1]
        return output

    def seek_namespaces(self, idx, lines):
        output = []
        for i in range(idx):
            line = lines[i].strip().rstrip("}").rstrip("\n")
            if line.startswith("namespace"):
                ar = line.split(" ")
                output.append(ar[ar.index("namespace") + 1])
        return output

    def process_closed_pragma(self, i, j, lines, pragmatype):
        if (pragmatype=="SerialIoStructStart"):
            output_object = HwppSerialIoStruct()
            output_object.define_struct_name(self.get_pragma_arg(lines[i], pragmatype, 0))
            relevant_namespaces = self.seek_namespaces(i, lines)
            output_object.define_namespaces(relevant_namespaces)
            for z in range(i+1, j-1):
                line = lines[z]
                if self.is_hwpp_pragma_invocation_type(line):
                    intermediate_pragmatype = self.get_pragma_type(line)
                    if intermediate_pragmatype == "SerialIoMember":
                        output_object.add_member(self.get_pragma_arg(lines[z], intermediate_pragmatype, 0), self.get_pragma_arg(lines[z], intermediate_pragmatype, 1))
            return output_object



    def check_closure(self, i, lines, pragmatype):
        outputval = -1
        for z in range(i, len(lines)):
            line = lines[z]
            if self.is_hwpp_pragma_invocation_type(line):
                pragmatype_loc = self.get_pragma_type(line)
                if pragmatype_loc == self.get_closing_pragma(pragmatype):
                    outputval = z
                    return outputval
        return outputval

    def get_closing_pragma(self, pragmatype):
        if (pragmatype == "SerialIoStructStart"):
            return "SerialIoStructEnd"
        return None


    def get_pragma_type(self, line):
        ar = line.strip().split(" ")
        return ar[ar.index("HWPP")+1]

    def is_hwpp_pragma_invocation_type(self, line):
        return line.strip().startswith("#pragma HWPP")

    def get_error_message(self):
        line_num_str = ""
        if self.error_line > 0:
            line_num_str = ", (line {})".format(str(self.error_line))
        return "HWPP: Error in {}{}: {}".format(self.basename, line_num_str, self.build_error)

    def build(self):
        print(">> Processing {}".format(self.basename))

class HwppOutputObject:
    def __init__(self, output_file_in):
        self.output_code = []
        self.output_file = output_file_in

    def build_from_objects(self, obj_list):
        self.make_header()
        for obj in obj_list:
            if obj.is_hwpp_dependency:
                self.output_code.append("#include \"{}\"".format(obj.basename))
        for obj in obj_list:
            obj_code = obj.generate_code_lines()
            for line in obj_code:
                self.output_code.append(line)
        self.make_footer()
        fh = open(self.output_file, "w")
        fh.writelines("\n".join(self.output_code))
        fh.close()

    def make_header(self):
        self.output_code.append("#ifndef HWPP_AUTO_H")
        self.output_code.append("#define HWPP_AUTO_H")
        self.output_code.append("")
        self.output_code.append("//-------------------------------------")
        self.output_code.append("//This is automatically generated code.")
        self.output_code.append("//Any changes made to this file will be")
        self.output_code.append("//Lost on the next build.")
        self.output_code.append("//-------------------------------------")

    def make_footer(self):
        self.output_code.append("#endif")

class HwppMacro(ABC):
    @abstractmethod
    def __init__(self):
        pass

    @abstractmethod
    def get_code_lines(self):
        pass

class HwppSerialIoStruct(HwppMacro):
    def __init__(self):
        self.dummy_name = "object"
        self.struct_name = None
        self.member_default_values = {}
        self.namespaces = []

    def define_struct_name(self, name_in):
        self.struct_name = name_in

    def define_namespaces(self, namespaces_in):
        self.namespaces = namespaces_in

    def add_member(self, name_in, val_in):
        self.member_default_values[name_in] = val_in

    def get_code_lines(self):
        output = []
        ftablevel = "".rjust(len(self.namespaces), "\t")
        for nlevel in range(len(self.namespaces)):
            tablevel = "".rjust(nlevel, "\t")
            output.append(tablevel + "namespace {}".format(self.namespaces[nlevel]))
            output.append(tablevel + "{")
        output.append(ftablevel + "void HWPP_SetDefaultValues({}* {})".format(self.struct_name, self.dummy_name))
        output.append(ftablevel + "{")
        for memname in self.member_default_values.keys():
            output.append(ftablevel + "\t{}->{} = {};".format(self.dummy_name, memname, self.member_default_values[memname]))
        output.append(ftablevel + "}")
        for nlevel in range(len(self.namespaces)):
            tablevel = "".rjust(nlevel, "\t")
            output.append(tablevel + "}")
        return output
