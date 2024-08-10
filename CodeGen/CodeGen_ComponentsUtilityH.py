import os

components_folder = "EngineCore/EngineCore/Components"
output_file = "EngineCore/EngineCore/Utilities/ComponentsUtility.h"

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
    
    f.write("\n")
    f.write("#include \"EngineCore/Core/Ref.h\"\n")
    f.write("#include \"EngineCore/Core/Entity.h\"\n")
    f.write("#include \"EngineCore/Core/IComponent.h\"\n")
    f.write("\n")

    for component in components:
        f.write(f"#include \"EngineCore/Components/{component}\"\n")

#Notes
    f.write("\n")
    f.write("/// <summary>\n")
    f.write("/// This script is autognerated\n")
    f.write("/// Don't make any changes in the file\n")
    f.write("/// </summary>")
    f.write("\n")

#Components array
    f.write("\n")
    f.write(f"const int componentsLength = {len(components)};\n")
    f.write("extern const char* allComponentsNames[componentsLength];\n")
    f.write("\n")

#Methods
    f.write("\n")
    f.write("bool HasComponent(Ref<Entity> entity, const std::string& componentName);\n")
    f.write("void AddComponent(Ref<Entity> entity, const std::string& componentName);\n")
    f.write("void RemoveComponent(Ref<Entity> entity, const std::string& componentName);\n")

print(f"Generated C++ for {output_file}")