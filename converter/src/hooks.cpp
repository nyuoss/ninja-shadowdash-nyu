 
void HandleRule(Rule* rule)
{
    // output string
    std::string output = "auto " + rule.name_ + " = rule{{bind(command, \"g++\", \"flags\"_v, \"-c\", ";

    // Iterate over the parsed_ vector in the "command" binding
    for (const auto& entry : rule.bindings_["command"].parsed_) {
        if (entry.second == RAW) {
            output += "\"" + entry.first + "\", ";  // Add quotes for raw strings
        } else if (entry.second == SPECIAL) {
            output += entry.first + ", ";  // No quotes for special variables
        }
    }

    // Finish the string
    output += ")}};";

    // Print the result
    std::cout << output << std::endl;
}
