import sys
import os
import PreProcessObjects as hwpp

target_location = os.getenv("HWPP_OUT")
print("Creating HWPP resource {}...".format(target_location))
object_sources = sys.argv[1:]
all_objects = []
for object in object_sources:
    build_object = hwpp.PreProcessorSourceObject(object)
    all_objects.append(build_object)
    passed = build_object.analyze()
    if passed:
        build_object.build()
    else:
        print(build_object.get_error_message())
        exit(166)
output_object = hwpp.HwppOutputObject(target_location)
output_object.build_from_objects(all_objects)
