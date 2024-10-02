import os

components_folder = "EngineCore/EngineCore/Components"
output_file = "EngineCore/EngineCore/Utilities/ComponentsUtility.cpp"

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
    f.write("#include \"EngineCore/Pch.h\"\n")
    f.write("#include \"EngineCore/Utilities/ComponentsUtility.h\"\n")
    f.write("\n")

#Notes
    f.write("\n")
    f.write("/// <summary>\n")
    f.write("/// This script is autognerated\n")
    f.write("/// Don't make any changes in the file\n")
    f.write("/// </summary>")
    f.write("\n")

#Components array
    f.write("\n")
    f.write("const char* allComponentsNames[componentsLength] =\n")
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