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
        formats: [ 'p' ]
    }, {
        name: '_Bool',
        signed: true,
        formats: [ 'b' ],
        alt_formats: [ 'd' ]
    }, {
        name: 'default',
        alt_name: 'void *',
        ptr: true,
        formats: [ 'unknown' ],
        alt_formats: [ 'p' ]
    }
];

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

    const postfix = type_info.name.endsWith('*') ? '' : ' ';

    return `\nATT_API unsigned int ${fn}(${type}${postfix}result, ${type}${postfix}expected, att_op op, const char *description, const char *file, unsigned int line)`;
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

        if(expected < 32 || expected == 127) {
            format_1 = "\\\\x%02X";
        } else {
            format_1 = "%c";
        }

        if(result < 32 || result == 127) {
            format_2 = "\\\\x%02X";
        } else {
            format_2 = "%c";
        }

        ATT_ERROR_MESSAGE(result, format_1, format_2, expected, op);`;
    } else if(s_format === '"%p"' && format_name(type_info, unsigned) === 'c') {
        error_message = `ATT_ERROR_MESSAGE(result, ATT_STRING_AS_POINTERS == 1 ? "%p" : "\\"%s\\"", ATT_STRING_AS_POINTERS == 1 ? "%p" : "\\"%s\\"", expected, op);`
    } else {
        error_message = `ATT_ERROR_MESSAGE(result, ${s_format}, ${s_format}, expected, op);`;
    }

    let body = '';

    if(name === 'default') {
        // Custom callbacks receive the operator; without a callback only EQ/NE
        // are meaningful on an unknown type.
        body = `    int test;

    if(att_generic_callback_fn) {
        test = att_assert("callback", att_generic_callback_fn(result, expected, op, description), description);
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
    } else if(name === 'void *' || name === '_Bool') {
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
let substitution = `
#ifndef ATT_CUSTOM_TYPES
#define ATT_CUSTOM_TYPES
#endif

// clang-format off
#ifndef __cplusplus
#define ATT_ASSERT_OP(VALUE, EXPECTED, OP, MESSAGE) \\
    (att_set_assert_context(#VALUE, __FILE__, __LINE__), _Generic(VALUE, \\
    ATT_CUSTOM_TYPES \\
    ${generics.join(', \\\n    ')} \\
)(VALUE, EXPECTED, OP, MESSAGE, __FILE__, __LINE__))
#else
#define ATT_ASSERT_OP(VALUE, EXPECTED, OP, MESSAGE) \\
    (att_set_assert_context(#VALUE, __FILE__, __LINE__), \\
     att_assert_cpp(VALUE, EXPECTED, OP, MESSAGE, __FILE__, __LINE__))
#endif

#define ATT_ASSERT(VALUE, EXPECTED, MESSAGE) ATT_ASSERT_OP(VALUE, EXPECTED, ATT_OP_EQ, MESSAGE)
#define ATT_ASSERT_NE(VALUE, EXPECTED, MESSAGE) ATT_ASSERT_OP(VALUE, EXPECTED, ATT_OP_NE, MESSAGE)
#define ATT_ASSERT_LT(VALUE, EXPECTED, MESSAGE) ATT_ASSERT_OP(VALUE, EXPECTED, ATT_OP_LT, MESSAGE)
#define ATT_ASSERT_LE(VALUE, EXPECTED, MESSAGE) ATT_ASSERT_OP(VALUE, EXPECTED, ATT_OP_LE, MESSAGE)
#define ATT_ASSERT_GT(VALUE, EXPECTED, MESSAGE) ATT_ASSERT_OP(VALUE, EXPECTED, ATT_OP_GT, MESSAGE)
#define ATT_ASSERT_GE(VALUE, EXPECTED, MESSAGE) ATT_ASSERT_OP(VALUE, EXPECTED, ATT_OP_GE, MESSAGE)\n`;

substitution += decls.join(';');
substitution += ';';

// Declarations and the ATT_ASSERT macro
file_content = substitute_text(
    file_content, "#define ATT_STRING_AS_POINTERS 0\n#endif\n",
    "\n\nunsigned int att_get_valid_tests(void);",
    substitution);

// Function bodies in the ATT_IMPLEMENTATION section
file_content = substitute_text(
    file_content, "int att_assert(const char *type, int test, const char *description);\n",
    "\n\n// clang-format on",
    contents.join('\n'));

writeFileSync('./attractor.h', file_content);
