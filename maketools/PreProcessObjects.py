import os
class PreProcessorSourceObject:
    def __init__(self, source_in):
        self.source = source_in
        self.full_file_name_no_ext, self.extension = os.path.splitext(source_in)
        self.basename = os.path.basename(source_in)
        self.build_error = "success"
        self.error_line = -1
        self.has_any_preprocessor_usage = False

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
        output.append("")
        output.append("//End {}".format(self.basename))
        output.append("")
        return output

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
