import os

components_folder = "EngineShared/Source/EngineShared/Components"
output_file = "EngineShared/Source/EngineShared/ComponentsUtility.h"

def extract_component_names(folder):
    component_names = []
    for filename in os.listdir(folder):
        if filename.endswith(".h"):
            component_names.append(filename)
    return component_names

components = extract_component_names(components_folder)

with open(output_file, 'w') as f:

#Defs
    f.write("\n")
    f.write("#pragma once\n")
    f.write("#include <string>\n")
    f.write("#include <vector>\n")
    f.write("#include <stdexcept>\n")

    f.write("#include \"Ref.h\"\n")
    f.write("#include \"Entity.h\"\n")
    f.write("#include \"IComponent.h\"\n")
    f.write("\n")

    for component in components:
        f.write(f"#include \"Components/{component}\"\n")

#Notes
    f.write("\n")
    f.write("/// <summary>\n")
    f.write("/// This script is autognerated\n")
    f.write("/// Don't make any changes in the file\n")
    f.write("/// </summary>")
    f.write("\n")

#Components array
    f.write("\n")
    f.write(f"const int componentsLength = {len(components)};")
    f.write("\n")
    f.write("\n")

    f.write("const char* componentsNames[componentsLength] =\n")
    f.write("{\n")
    for component in components:
        f.write(f"      \"{component.split('.')[0].strip()}\",\n")
    f.write("};\n")
    f.write("\n")

#Has component method
    f.write("\n")
    f.write("bool HasComponent(Ref<Entity> entity, const std::string& componentName)\n")
    f.write("{\n")

    for component in components:
        f.write(f"      if (componentName == \"{component.split('.')[0].strip()}\") return entity->HasComponent<{component.split('.')[0].strip()}>();\n")
    
    f.write(f"      throw std::runtime_error(\"Not supported component type: \" + componentName);\n")
    f.write("}\n")
    f.write("\n")
#Add component method

    f.write("\n")
    f.write("void AddComponent(Ref<Entity> entity, const std::string& componentName)\n")
    f.write("{\n")
    f.write("      if (HasComponent(entity, componentName)) return;\n")
    f.write("\n")
    
    for component in components:
        f.write(f"      if (componentName == \"{component.split('.')[0].strip()}\") entity->AddComponent<{component.split('.')[0].strip()}>();\n")

    f.write("}\n")
    f.write("\n")

#Remove component method

    f.write("\n")
    f.write("void RemoveComponent(Ref<Entity> entity, const std::string& componentName)\n")
    f.write("{\n")
    f.write("      if (!HasComponent(entity, componentName)) return;\n")
    f.write("\n")
    
    for component in components:
        f.write(f"      if (componentName == \"{component.split('.')[0].strip()}\") entity->RemoveComponent<{component.split('.')[0].strip()}>();\n")

    f.write("}\n")
    f.write("\n")

print(f"Generated C++ for {output_file}")