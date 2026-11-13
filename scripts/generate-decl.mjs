/**
 * The Attractor Unit Test library
 *
 * This file is distributed under the MIT License. See LICENSE for details.
 */

import { readFileSync, writeFileSync } from 'fs';

const types = [
    {
        name: 'char',
        signed: true,
        unsigned: true,
        ptr: true,
        const_ptr: true,
        formats: [ 'c', 'c' ]
    }, {
        name: 'short',
        signed: true,
        unsigned: true,
        formats: [ 'hd', 'hu' ]
    }, {
        name: 'int',
        signed: true,
        unsigned: true,
        formats: [ 'd', 'u' ]
    }, {
        name: 'long',
        signed: true,
        unsigned: true,
        formats: [ 'ld', 'lu' ]
    }, {
        name: 'long long',
        signed: true,
        unsigned: true,
        formats: [ 'lld', 'llu' ]
    }, {
        name: 'float',
        signed: true,
        formats: [ 'f' ],
        error_formats: [ '.9g' ]
    }, {
        name: 'double',
        signed: true,
        formats: [ 'lf' ],
        error_formats: [ '.17g' ]
    }, {
        name: 'long double',
        signed: true,
        formats: [ 'Lf' ],
        error_formats: [ '.21Lg' ]
    }, {
        name: 'void',
        ptr: true,
        const_ptr: true,
        formats: [ 'p' ]
    }, {
        name: '_Bool',
        signed: true,
        formats: [ 'b' ],
        alt_formats: [ 'd' ]
    }, {
        name: 'default',
        alt_name: 'const void *',
        ptr: true,
        formats: [ 'unknown' ],
        alt_formats: [ 'p' ]
    }
];

const mixed_types = [ 'short', 'int', 'long', 'long long' ];

const generics = [];
const decls = [];
const contents = [];

function substitute_text(file_content, start, end, replacement) {
    const start_index = file_content.indexOf(start) + start.length;
    const end_index = end === null ? null : file_content.indexOf(end, start_index);

    const ret = file_content.slice(0, start_index) + replacement;

    if(end_index !== null) {
      return ret + file_content.slice(end_index);
    } else {
      return ret + "\n";
    }
  }

function type_name(type_info, unsigned) {
    return (unsigned ? 'unsigned ': '') + type_info.name;
}

function format_name(type_info, unsigned, alt) {
    const index = unsigned ? 1 : 0;

    if(alt && type_info.alt_formats) {
        return type_info.alt_formats[index];
    }

    return type_info.formats[index];
}

function fn_name(type_info, unsigned, is_const) {
    const format = format_name(type_info, unsigned);
    let pointer = '';

    if(type_info.name.endsWith('*') && format !== 'unknown') {
        pointer = is_const ? '_cp' : '_p';
    }

    return `att_assert${unsigned ? '_u' : ''}${pointer}_${format}`;
}

function fn_decl(type_info, unsigned, is_const) {
    let type = type_name(type_info, unsigned);
    const fn = fn_name(type_info, unsigned, is_const);

    if(type_info.alt_name) {
        type = type_info.alt_name;
    }

    // <stdbool.h> spells the type the same way in C and C++, which has no _Bool.
    if(type === '_Bool') {
        type = 'bool';
    }

    // A string is only read, so "char *" accepts a "const char *" expected value too.
    if(type === 'char *') {
        type = 'const char *';
    }

    const sep = type.endsWith('*') ? '' : ' ';

    return `\nATT_API unsigned int ${fn}(${type}${sep}result, ${type}${sep}expected, att_op op, const char *description, const char *file, unsigned int line)`;
}

function fn_content(type_info, unsigned, is_const) {
    const format = format_name(type_info, unsigned, true);
    const e_format = type_info.error_formats ?
        type_info.error_formats[unsigned ? 1 : 0] :
        (type_info.name.endsWith('*') ? 'p' : format);
    const s_format = '"%' + e_format + '"';
    const type = '"' + type_name(type_info, unsigned) + '"';
    const name = type_info.name;
    let error_message = '';

    if(s_format === '"%c"') {
        error_message = `const char *format_1;\n        const char *format_2;

        // Go through unsigned char: a negative char must not be sign-extended, and
        // bytes outside printable ASCII are shown as hex.
        if((unsigned char)expected < 32 || (unsigned char)expected >= 127) {
            format_1 = "\\\\x%02X";
        } else {
            format_1 = "%c";
        }

        if((unsigned char)result < 32 || (unsigned char)result >= 127) {
            format_2 = "\\\\x%02X";
        } else {
            format_2 = "%c";
        }

        ATT_ERROR_MESSAGE((unsigned char)result, format_1, format_2, (unsigned char)expected, op);`;
    } else if(s_format === '"%p"' && format_name(type_info, unsigned) === 'c') {
        error_message = `if(ATT_STRING_AS_POINTERS == 1) {
            ATT_ERROR_MESSAGE((const void *)result, "%p", "%p", (const void *)expected, op);
        } else {
            // A NULL string is printed as an unquoted NULL instead of being passed to %s.
            ATT_ERROR_MESSAGE(result ? result : "NULL", expected ? "\\"%s\\"" : "%s", result ? "\\"%s\\"" : "%s", expected ? expected : "NULL", op);
        }`;
    } else {
        error_message = `ATT_ERROR_MESSAGE(result, ${s_format}, ${s_format}, expected, op);`;
    }

    let body = '';

    if(name === 'default') {
        // Custom callbacks receive the operator; without a callback only EQ/NE
        // are meaningful on an unknown type.
        body = `    int test;

    if(att_generic_callback_fn) {
        test = att_assert("callback", att_generic_callback_fn((void *)result, (void *)expected, op, description), description);
    } else if(op != ATT_OP_EQ && op != ATT_OP_NE) {
        return att_assert_op_unsupported(op, description, file, line);
    } else {
        test = att_assert("default", op == ATT_OP_EQ ? result == expected : result != expected, description);
    }`;
    } else if(name === 'char *' || name === 'const char *') {
        // Lexicographic comparison via strcmp. Relational operators on a NULL
        // string always fail; EQ/NE fall back to pointer identity.
        body = `    int test;

    if(result && expected) {
        test = ATT_COMPARE(strcmp(result, expected), 0, op);
    } else {
        test = op == ATT_OP_EQ ? result == expected : (op == ATT_OP_NE ? result != expected : 0);
    }

    test = att_assert(${type}, test, description);`;
    } else if(name === 'float' || name === 'double' || name === 'long double') {
        // EQ/NE honor the configured epsilon; the relational operators compare plainly.
        body = `    int equal = (result == expected) || ((result > expected ? result - expected : expected - result) <= att_float_epsilon);
    int test = att_assert(${type}, op == ATT_OP_EQ ? equal : (op == ATT_OP_NE ? !equal : ATT_COMPARE(result, expected, op)), description);`;
    } else if(name === 'void *' || name === 'const void *' || name === '_Bool') {
        body = `    if(op != ATT_OP_EQ && op != ATT_OP_NE) {
        return att_assert_op_unsupported(op, description, file, line);
    }

    int test = att_assert(${type}, op == ATT_OP_EQ ? result == expected : result != expected, description);`;
    } else {
        body = `    int test = att_assert(${type}, ATT_COMPARE(result, expected, op), description);`;
    }

    return fn_decl(type_info, unsigned, is_const) + ` {
${body}

    if(!test) {
        ${error_message}
    }

    return test;
}`;
}

function generic_decl(type_info, unsigned, is_const) {
    const type = type_name(type_info, unsigned);
    const fn = fn_name(type_info, unsigned, is_const);

    if(mixed_types.includes(type_info.name)) {
        return `${type}: ${unsigned ? 'ATT_GENERIC_UNSIGNED' : 'ATT_GENERIC_SIGNED'}(EXPECTED, ${fn})`;
    }

    return `${type}: ${fn}`;
}

for(const type_info of types) {
    if(type_info.signed) {
        decls.push(fn_decl(type_info, false, false));
        generics.push(generic_decl(type_info, false, false));
        contents.push(fn_content(type_info, false, false));
    }

    if(type_info.unsigned) {
        decls.push(fn_decl(type_info, true, false));
        generics.push(generic_decl(type_info, true, false));
        contents.push(fn_content(type_info, true, false));
    }

    const name = type_info.name;

    if(type_info.ptr) {
        type_info.name = name === 'default' ? name : name + ' *';
        decls.push(fn_decl(type_info, false, false));
        generics.push(generic_decl(type_info, false, false));
        contents.push(fn_content(type_info, false, false));
    }

    if(type_info.const_ptr) {
        type_info.name = 'const ' + name + ' *';

        decls.push(fn_decl(type_info, false, true));
        generics.push(generic_decl(type_info, false, true));
        contents.push(fn_content(type_info, false, true));
    }
}

let file_content = readFileSync('./attractor.h', 'utf-8');

// Declarations of the ATT_ASSERT_OP macro.
file_content = substitute_text(
    file_content, "ATT_CUSTOM_TYPES \\\n    ",
    ")(VALUE, EXPECTED, OP, MESSAGE, __FILE__, __LINE__))",
    generics.join(', \\\n    ') + ' \\\n');

// Declarations of the functions list.
file_content = substitute_text(
    file_content, "// This functions list is automatically generated. Do not edit.",
    "\n\nATT_API unsigned int att_get_valid_tests(void);",
    decls.join(';') + ';');

// Function bodies in the ATT_IMPLEMENTATION section
file_content = substitute_text(
    file_content, "// These functions are automatically generated. Do not edit.",
    "\n#endif /* ATT_IMPLEMENTATION_INCLUDED */",
    contents.join('\n'));

writeFileSync('./attractor.h', file_content);
