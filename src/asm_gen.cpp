#include "asm.hpp"
#include "tac.hpp"
#include "data_structures.hpp"
#include "x86_32_lib.hpp"

extern std::string outputDir;
void print_error(const std::string &message);

std::ofstream asm_file;

std::map<std::string, int> string_literals;
bool inside_fn = false;

// // Structure to store alignment information
// struct AlignmentInfo
// {
//     std::map<std::string, ull> memberOffsets; // Aligned member offsets
//     ull totalSize;                            // Total size with padding
// };

// // Maps to store alignment information for structs and classes
// std::map<std::string, AlignmentInfo> structAlignments;
// std::map<std::string, AlignmentInfo> classAlignments;

// // Calculate aligned offset based on type and current offset
// ull align_offset(ull current_offset, std::string type)
// {
//     // Default alignment is 4 bytes (for most types)
//     ull alignment = 4;

//     // Determine alignment based on type
//     if (type == "char")
//     {
//         alignment = 1;
//     }
//     else if (type == "short")
//     {
//         alignment = 2;
//     }
//     else if (type == "int" || type == "float" || type.find("*") != std::string::npos)
//     {
//         alignment = 4;
//     }
//     else if (type == "double" || type == "long long")
//     {
//         alignment = 8;
//     }
//     else if (type.substr(0, 7) == "STRUCT_")
//     {
//         std::string struct_name = type.substr(7);
//         // If we've already calculated alignment for this struct, use it
//         if (structAlignments.find(struct_name) != structAlignments.end())
//         {
//             // For nested structs, use the largest alignment of any member
//             // This is a simplification - a more accurate approach would be to track the max alignment
//             alignment = 4; // Default to 4 for structs if we don't have better info
//         }
//     }
//     else if (type.substr(0, 6) == "CLASS_")
//     {
//         // Similar logic for classes
//         alignment = 4;
//     }

//     // Calculate padding needed to align correctly
//     ull padding = (alignment - (current_offset % alignment)) % alignment;
//     return current_offset + padding;
// }

// // Calculate proper struct alignment and padding
// void calculate_struct_alignment(std::string struct_name)
// {
//     struct_sym_table *temp = curr_struct_table;
//     while (temp)
//     {
//         if ((*temp).find(struct_name) != (*temp).end())
//         {
//             sym_table *struct_table = (*temp)[struct_name].second;
//             AlignmentInfo info;
//             ull current_offset = 0;

//             // First pass: calculate aligned offsets for each member
//             for (auto &member : (*struct_table))
//             {
//                 // Align this member
//                 current_offset = align_offset(current_offset, member.second->type);

//                 // Store the aligned offset
//                 info.memberOffsets[member.first] = current_offset;

//                 // Move offset past this member
//                 current_offset += member.second->size;
//             }

//             // Calculate final struct size with proper alignment
//             // Structs typically align to their most strictly aligned member
//             ull struct_alignment = 4;                             // Default to 4-byte alignment
//             info.totalSize = align_offset(current_offset, "int"); // Align to int boundary by default

//             // Store the alignment info for this struct
//             structAlignments[struct_name] = info;
//             return;
//         }
//         if (struct_parent_table.find(temp) == struct_parent_table.end())
//             break;
//         temp = struct_parent_table[temp];
//     }
// }

// // Calculate proper class alignment and padding
// void calculate_class_alignment(std::string class_name)
// {
//     class_sym_table *temp = curr_class_table;
//     while (temp)
//     {
//         if ((*temp).find(class_name) != (*temp).end())
//         {
//             sym_table *class_table = (*temp)[class_name].second;
//             AlignmentInfo info;
//             ull current_offset = 0;

//             // First pass: calculate aligned offsets for each member
//             for (auto &member : (*class_table))
//             {
//                 // Skip methods (they don't need memory in the object)
//                 if (member.first.find("FUNC_") == 0)
//                 {
//                     continue;
//                 }

//                 // Align this member
//                 current_offset = align_offset(current_offset, member.second->type);

//                 // Store the aligned offset
//                 info.memberOffsets[member.first] = current_offset;

//                 // Move offset past this member
//                 current_offset += member.second->size;
//             }

//             // Calculate final class size with proper alignment
//             ull class_alignment = 4;                              // Default to 4-byte alignment
//             info.totalSize = align_offset(current_offset, "int"); // Align to int boundary

//             // Store the alignment info for this class
//             classAlignments[class_name] = info;
//             return;
//         }
//         if (class_parent_table.find(temp) == class_parent_table.end())
//             break;
//         temp = class_parent_table[temp];
//     }
// }

// // Calculate all struct and class alignments before code generation
// void calculate_all_alignments()
// {
//     // Calculate alignments for all structs
//     struct_sym_table *struct_temp = curr_struct_table;
//     while (struct_temp)
//     {
//         for (auto &s : (*struct_temp))
//         {
//             calculate_struct_alignment(s.first);
//         }
//         if (struct_parent_table.find(struct_temp) == struct_parent_table.end())
//             break;
//         struct_temp = struct_parent_table[struct_temp];
//     }

//     // Calculate alignments for all classes
//     class_sym_table *class_temp = curr_class_table;
//     while (class_temp)
//     {
//         for (auto &c : (*class_temp))
//         {
//             calculate_class_alignment(c.first);
//         }
//         if (class_parent_table.find(class_temp) == class_parent_table.end())
//             break;
//         class_temp = class_parent_table[class_temp];
//     }
// }

// // Get the aligned size of a struct or class
// ull get_aligned_size(std::string type_name)
// {
//     if (type_name.substr(0, 7) == "STRUCT_")
//     {
//         std::string struct_name = type_name.substr(7);
//         if (structAlignments.find(struct_name) != structAlignments.end())
//             return structAlignments[struct_name].totalSize;
//     }
//     else if (type_name.substr(0, 6) == "CLASS_")
//     {
//         std::string class_name = type_name.substr(6);
//         if (classAlignments.find(class_name) != classAlignments.end())
//             return classAlignments[class_name].totalSize;
//     }

//     // If not found or not a struct/class, return the regular size
//     return getSize(type_name);
// }

// // Get the aligned member offset
// ull get_aligned_member_offset(std::string container_type, std::string member_name)
// {
//     if (container_type.substr(0, 7) == "STRUCT_")
//     {
//         std::string struct_name = container_type.substr(7);
//         if (structAlignments.find(struct_name) != structAlignments.end() &&
//             structAlignments[struct_name].memberOffsets.find(member_name) != structAlignments[struct_name].memberOffsets.end())
//             return structAlignments[struct_name].memberOffsets[member_name];
//     }
//     else if (container_type.substr(0, 6) == "CLASS_")
//     {
//         std::string class_name = container_type.substr(6);
//         if (classAlignments.find(class_name) != classAlignments.end() &&
//             classAlignments[class_name].memberOffsets.find(member_name) != classAlignments[class_name].memberOffsets.end())
//             return classAlignments[class_name].memberOffsets[member_name];
//     }

//     // If not found, return 0 (error case)
//     return 0;
// }

void emit_asm(const std::string &inputFile)
{
    asm_file = std::ofstream(outputDir + inputFile + ".asm");
    if (!asm_file)
    {
        print_error("cannot open " + outputDir + inputFile + ".asm");
        return;
    }

    if (tac_code.empty())
        return;
    if (!optimize_ir)
        addgotoLabels();

    get_string_literals();
    global_init_pass();
    compute_basic_blocks();
    print_basic_blocks();
    // Calculate proper alignment for all structs and classes
    // calculate_all_alignments();

    add_extern_funcs();
    emit_section(".text");
    emit_data("global main");
    
    for (auto &block : basic_blocks)
    {
        next_use_analysis(block);
        for (auto &instr : block)
        {
            const std::string curr_op = instr.op;
            if (curr_op.substr(0, 5) == "FUNC_")
            {
                if (curr_op.substr(curr_op.length() - 7) == "start :")
                {
                    inside_fn = true; // TODO : comment if useless
                    if (curr_op.substr(5, 5) == "4main")
                        emit_label("\nmain ");
                    else
                        emit_label("\n" + curr_op.substr(0, curr_op.length() - 7));
                    // TODO : add function prologue
                }
                else
                    inside_fn = false; // TODO : comment if useless
            }
            else if (curr_op.back() == ':')
                emit_label(curr_op.substr(0, curr_op.length() - 1));
            else if (curr_op == "=" || curr_op == "(f)=")
                emit_assign(instr);
        }
    }

    emit_section(".data");
    print_data_section(); // add initialized data
    emit_section(".bss");
    print_bss_section(); // add uninitialized data
    print_next_use(); 
}

void get_string_literals()
{
    int str_cnt = 0;
    for (auto &instr : tac_code)
    {
        if (instr.arg1.value.length() > 0 && instr.arg1.value[0] == '\"')
        {
            if (string_literals.find(instr.arg1.value) == string_literals.end())
                string_literals[instr.arg1.value] = str_cnt++;
            instr.arg1.value = "__str_" + std::to_string(string_literals[instr.arg1.value]);
        }
        if (instr.arg2.value.length() > 0 && instr.arg2.value[0] == '\"')
        {
            if (string_literals.find(instr.arg2.value) == string_literals.end())
                string_literals[instr.arg2.value] = str_cnt++;
            instr.arg2.value = "__str_" + std::to_string(string_literals[instr.arg2.value]);
        }
        if (instr.result.value.length() > 0 && instr.result.value[0] == '\"')
        {
            if (string_literals.find(instr.result.value) == string_literals.end())
                string_literals[instr.result.value] = str_cnt++;
            instr.result.value = "__str_" + std::to_string(string_literals[instr.result.value]);
        }
    }
}

void global_init_pass()
{
    for (const auto &instr : tac_code)
    {
        const std::string curr_op = instr.op;
        if (curr_op.substr(0, 5) == "FUNC_")
        {
            if (curr_op.substr(curr_op.length() - 7) == "start :")
                inside_fn = true;
            else
                inside_fn = false;
        }
        else if (curr_op == "=" || curr_op == "(f)=")
        {
            if (!inside_fn)
                global_init[instr.result.value] = instr.arg1.value;
        }
    }

    // Replace enums with their corresponding values
    for (auto &instr : tac_code)
    {
        if (instr.arg1.entry && instr.arg1.entry->isEnum)
        {
            instr.arg1.value = global_init[instr.arg1.value];
            instr.arg1.entry->isEnum = false;
            instr.arg1.entry->isGlobal = false;
        }
        if (instr.arg2.entry && instr.arg2.entry->isEnum)
        {
            instr.arg2.value = global_init[instr.arg2.value];
            instr.arg2.entry->isEnum = false;
            instr.arg2.entry->isGlobal = false;
        }
    }
}

void print_data_section()
{
    for (auto &[name, entry] : gst)
    {
        if (entry->isGlobal && !entry->isEnum && entry->init)
        {
            if (entry->type == "char")
                emit_data(name + ": db " + global_init[name]);
            else if (entry->type == "int" || entry->type == "float")
                emit_data(name + ": dd " + global_init[name]);
            else if (entry->type == "char*")
            {
                if (entry->isArray)
                {
                    // Initialized array pending as of now
                }
                else
                {
                    std::string init_val = global_init[name];
                    emit_data(name + ": dd " + init_val);
                }
            }
            else if (entry->type == "int*" || entry->type == "float*")
            {
                if (entry->isArray)
                {
                    // Initialized array pending as of now
                }
                else
                {
                    std::string init_val = global_init[name];
                    emit_data(name + ": dd " + init_val);
                }
            }
            // Initialized structs, unions and classes pending as of now
        }
    }
    for (auto i = 0; i < string_literals.size(); i++)
    {
        for (const auto &[str, cnt] : string_literals)
        {
            if (cnt == i)
                emit_data("__str_" + std::to_string(cnt) + ": db " + str + ", 0");
        }
    }
}

void print_bss_section()
{
    for (auto &[name, entry] : gst)
    {
        if (entry->isGlobal && !entry->isEnum && !entry->init)
        {
            if (entry->type == "char")
                emit_data(name + ": resb 1");
            else if (entry->type == "int" || entry->type == "float")
                emit_data(name + ": resd 1");
            else if (entry->type == "char*")
            {
                if (entry->isArray)
                    emit_data(name + ": resb " + std::to_string(entry->size));
                else
                    emit_data(name + ": resd 1");
            }
            else if (entry->type == "int*" || entry->type == "float*")
            {
                if (entry->isArray)
                    emit_data(name + ": resd " + std::to_string(entry->size / 4));
                else
                    emit_data(name + ": resd 1");
            }
            // TODO : padding??
            // else if (entry->type.substr(0, 7) == "STRUCT_" || entry->type.substr(0, 6) == "CLASS_" || entry->type.substr(0, 6) == "UNION_")
            // {
            //     if (entry->size == 1)
            //         emit_data(name + ": resb 1");
            //     else
            //         emit_data(name + ": resd " + std::to_string(entry->size / 4));
            // }
            // else if (entry->type.substr(0, 6) == "UNION_")
            // {
            //     if (entry->size == 1)
            //         emit_data(name + ": resb 1");
            //     else
            //         emit_data(name + ": resd " + std::to_string(entry->size / 4));
            // }
        }
    }
}

void next_use_analysis(std::vector<quad> &block)
{
    // Assume symbol table shows all non-temporary and temporaries variables in B as live on exit
    for(int j=0;j<block.size();j++){
        quad& I = block[j];
        if(I.arg1.entry != NULL){
            I.arg1.entry->isLive = 1;
            I.arg1.entry->nextUse = -1;
        }
        
        if(I.arg2.entry != NULL){
            I.arg2.entry->isLive = 1;
            I.arg2.entry->nextUse = -1;
        }
    
        if(I.result.entry != NULL){
            I.result.entry->isLive = 1;
            I.result.entry->nextUse = -1;
        }
    }

    for(int j=(int)block.size()-1;j>=0;j--){
        quad& I = block[j];
        if(I.arg1.entry != NULL){
            I.arg1.isLive = I.arg1.entry->isLive;
            I.arg1.nextUse = I.arg1.entry->nextUse;
        }
        
        if(I.arg2.entry != NULL){
            I.arg2.isLive = I.arg2.entry->isLive;
            I.arg2.nextUse = I.arg2.entry->nextUse;
        }

        if(I.result.entry != NULL){
            I.result.isLive = I.result.entry->isLive;
            I.result.nextUse = I.result.entry->nextUse;
        }

        if(I.arg1.entry != NULL){
            I.arg1.entry->isLive = 1;
            I.arg1.entry->nextUse = I.Label;
        }
        
        if(I.arg2.entry != NULL){
            I.arg2.entry->isLive = 1;
            I.arg2.entry->nextUse = I.Label;
        }

        if(I.result.entry != NULL){
            I.result.entry->isLive = 0;
            I.result.entry->nextUse = -1;
        }
    }
}

void print_next_use(){
    std::cout << "NEXT USE INFO\n";
    int block_no = 0;
    for (auto &block : basic_blocks)
    {
        std::cout << "Block " << block_no++ << ":\n";
        for (auto &instr : block)
        {
            std::cout << stringify(instr) << std::endl;
            if(instr.result.entry){
                std::cout << '\t' << instr.result.value << ": " << instr.result.isLive << ' ' << instr.result.nextUse << '\n';
            }
            if(instr.arg1.entry){
                std::cout << '\t' << instr.arg1.value << ": " << instr.arg1.isLive << ' ' << instr.arg1.nextUse << '\n';
            }
            if(instr.arg2.entry){
                std::cout << '\t' << instr.arg2.value << ": " << instr.arg2.isLive << ' ' << instr.arg2.nextUse << '\n';
            }
            // std::cout << '(' << (instr.arg1.entry != NULL) << ' ' << instr.arg1.isLive << ' ' << instr.arg1.nextUse << ')';
            // std::cout << '(' << (instr.arg2.entry != NULL) << ' ' << instr.arg2.isLive << ' ' << instr.arg2.nextUse << ')';
            // std::cout << '(' << (instr.result.entry != NULL) << ' ' << instr.result.isLive << ' ' << instr.result.nextUse << ")\n";
        }
        std::cout << "\n";
    }
}

void emit_assign(quad &instr)
{
    if (!inside_fn) // TODO : comment if better way found
        return;
    // TODO complete the function
}
