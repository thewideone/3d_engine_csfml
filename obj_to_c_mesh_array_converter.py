# .obj file to C-style array of vertices and faces converter.
# Intended to be used for my 3D graphics engine.
# Made by Szymon Kajda



# For mesh name extraction
import os
# For floating to fixed point number conversion
import numpy as np
# For "press any key to continue"
from sys import platform



#
# Default Config
#

print_to_std_out = 1
print_to_file = 1
face_fixed_vert_cnt = 0
use_fixed_point = 0
# Vertex types
floating_point_type = "flp_t"
fixed_point_type_name = "fxp_t"
fixed_point_type = "int32"
# Fixed point number of binary digits after decimal place
fp_dp = 16
# Define types of vertex and face arrays
vert_arr_type = floating_point_type
# Automatically adjusted
face_arr_type = "uint8_t"
# Define something separating IDs of vertices in face array
face_vid_sep = "F_VID_SEP"



#
# Main program
#

dirname = os.path.dirname(__file__)

# Specify input file
print( "Input file path: " )
user_in = input()
filename = os.path.join( dirname, user_in )
in_file = open(filename, 'r')

print( "Print to file? (y - yes, n - no (only here), b - both (default): ", end = '' )
user_in = input()

if user_in == 'y':
    print_to_std_out = 0
elif user_in == 'n':
    print_to_file = 0

# Specify output file
if print_to_file:
    print( "Output file path: " )
    user_in = input()
    filename = os.path.join( dirname, user_in )
    out_file = open(filename, 'w')

# Specify if faces consist of fixed number of vertices
print( "Fixed number of vertices in faces? (y - yes, n - no (default): ", end = '' )
user_in = input()

if user_in == 'y':
    face_fixed_vert_cnt = 1

# Specify vertex array type
print( "Vertex array type (fl - floating point (float or double) (default), fx - fixed point): ", end = '' )
user_in = input()

if user_in == "fx":
    use_fixed_point = 1
    vert_arr_type = fixed_point_type_name

    print( "Number of binary digits after the decimal place (16 default): ", end = '' )
    user_in = input()

    if user_in:
        fp_dp = user_in

    print( "Fixed point type (int8, int16, int32 (default), int64): ", end = '' )
    user_in = input()

    if user_in:
        if user_in != "int8" and user_in != "int16" and user_in != "int32" and user_in != "int64":
            print( "Wrong intput! Continuing with the default value" )
        else:
            fixed_point_type = user_in

# Specify vertex ID separator in face array
print( "Vertex ID separator in face array (default F_VID_SEP): ", end = '' )
user_in = input()

if user_in:
    face_vid_sep = user_in

# Define mesh name
print( "Custom mesh name? (if no, leave empty): ", end = '' )
user_in = input()

if user_in:
    mesh_name = user_in
else:
    mesh_name = os.path.basename( in_file.name ).split('.')[0]



# Read lines
in_lines = in_file.readlines()

vert_lines = []
face_lines = []

for line in in_lines:
    if line[0] == 'v':
        vert_lines.append(line)
    elif line[0] == 'f':
        face_lines.append(line)

last_vert_line = vert_lines[-1]
last_face_line = face_lines[-1]

# Assign type based on the greatest vertex ID (number of vertices)
if len(vert_lines) <= 255:
    face_arr_type = "uint8_t"
elif len(vert_lines) <= 65535:
    face_arr_type = "uint16_t"
elif len(vert_lines) <= 4294967295:
    face_arr_type = "uint32_t"
# Please don't XD
elif len(vert_lines) <= 18446744073709551615:
    face_arr_type = "uint64_t"



# Print header of vertex array
if print_to_file:
    out_file.write( "const "+vert_arr_type+" "+mesh_name+"_verts[] = {\n" )
if print_to_std_out:
    print( "const "+vert_arr_type+" "+mesh_name+"_verts[] = {" )

# Print vertices
for line in vert_lines:
    elements = line.split()
    
    if use_fixed_point:
        # Convert floating point values into fixed point ones
        values = np.array([], dtype=fixed_point_type)
        
        for val in elements[1:]:
            # What kind of int is it casted to?
            float_val = float(val) * float(1<<fp_dp) + ( 0.5 if float(val) >= 0 else -0.5 )

            if fixed_point_type == "int8":
                value = np.int8( float_val )
            elif fixed_point_type == "int16":
                value = np.int16( float_val )
            elif fixed_point_type == "int32":
                value = np.int32( float_val )
            elif fixed_point_type == "int64":
                value = np.int64( float_val )
            
            values = np.append( values, str(value) )

        if print_to_file:
            out_file.write( '\t' + ', '.join(values) )
        if print_to_std_out:
            print( '\t' +  ', '.join(values) , end = '' )

    else:
        if print_to_file:
            out_file.write( '\t' + ', '.join( elements[1:] ) )
        if print_to_std_out:
            print( '\t' + ', '.join( elements[1:] ), end = '' )
        
    if line != last_vert_line:
        if print_to_file:
            out_file.write( ',\n' )
        if print_to_std_out:
            print( ',' )
    else:
        if print_to_file:
            out_file.write( '\n' )
        if print_to_std_out:
            print( '' )

# Close the array
if print_to_file:
    out_file.write( "};\n" )
if print_to_std_out:
    print( "};" )



# Print header of face array
if print_to_file:
    out_file.write( "const "+face_arr_type+" "+mesh_name+"_faces[] = {\n" )
if print_to_std_out:
    print( "const "+face_arr_type+" "+mesh_name+"_faces[] = {" )

# Print faces
for line in face_lines:
    elements = line.split()

    # Subtract 1 from each vertex ID since indices in C start from 0
    elements[1:] = [*map( lambda x: str(int(x)-1), elements[1:] )]
        
    if print_to_file:
        out_file.write( '\t' + ', '.join( elements[1:] ) )
        if not face_fixed_vert_cnt:
            out_file.write( ",\t" + face_vid_sep )
            
    if print_to_std_out:
        print( '\t' + ', '.join( elements[1:] ), end = '' )
        if not face_fixed_vert_cnt:
            print( ",\t" + face_vid_sep, end = '' )

    if line != last_face_line:
        if print_to_file:
            out_file.write( ',\n' )
        if print_to_std_out:
            print( ',' )
    else:
        if print_to_file:
            out_file.write( '\n' )
        if print_to_std_out:
            print( '' )

# Close the array
if print_to_file:
    out_file.write( "};\n" )
if print_to_std_out:
    print( "};" )

# Close both files
in_file.close()
if print_to_file:
    out_file.close()
    

if platform.startswith('linux'):
    os.system("read")
else:
    os.system("pause")
