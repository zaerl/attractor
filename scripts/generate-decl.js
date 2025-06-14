// This is a script used to generate functions declarations for the Attractor Library
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
        formats: [ 'f' ]
    }, {
        name: 'double',
        signed: true,
        formats: [ 'lf' ]
    }, {
        name: 'long double',
        signed: true,
        formats: [ 'Lf' ]
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
        alt_name: 'void*',
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

    return `\nATT_API unsigned int ${fn}(${type} result, ${type} expected, const char *description)`;
}

function fn_content(type_info, unsigned, is_const) {
    let comparison = 'result == expected';
    const format = format_name(type_info, unsigned, true);
    let s_format = '"%' + (type_info.name.endsWith('*') ? 'p' : format) + '"';
    let type_name = `"${type_info.name}"`;

    if(type_info.name === 'default') {
        comparison = `att_callback ? att_callback(result, expected, description) : (${comparison})`;
        type_name = `att_callback ? "callback" : "default"`;
    } else if(type_info.name === 'char*') {
        comparison = '((result == expected) || ((result && expected) ? strcmp(result, expected) == 0 : 0))';
        s_format = 'ATT_STRING_AS_POINTERS == 1 ? "%p" : "\\"%s\\""';
    }

    return fn_decl(type_info, unsigned, is_const) + ` {
    int test = att_assert(${type_name}, ${comparison}, description);

    if(!test) {
        ATT_ERROR_MESSAGE(result, ${s_format}, expected);
    }

    return test;
}`;
}

function generic_decl(type_info, unsigned, is_const) {
    const type = type_name(type_info, unsigned);
    const fn = fn_name(type_info, unsigned, is_const);

    return `${type}: ${fn}`;
}

const generate_c = process.argv.length === 3 && process.argv[2] === 'c';

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
        type_info.name = name === 'default' ? name : name + '*';
        decls.push(fn_decl(type_info, false, false));
        generics.push(generic_decl(type_info, false, false));
        contents.push(fn_content(type_info, false, false));
    }

    if(type_info.const_ptr) {
        type_info.name = 'const ' + name + '*';

        decls.push(fn_decl(type_info, false, true));
        generics.push(generic_decl(type_info, false, true));
        contents.push(fn_content(type_info, false, true));
    }
}

if(generate_c) {
    let file_content = readFileSync('./attractor.c', 'utf-8');
    file_content = substitute_text(
        file_content, "int att_assert(const char *type, int test, const char *description);\n",
        "\n\nint att_assert(const char *format",
        contents.join('\n'));

    writeFileSync('./attractor.c', file_content);
} else {
    let file_content = readFileSync('./attractor.h', 'utf-8');
    let substitution = "\n#define ATT_ASSERT(VALUE, EXPECTED, MESSAGE) _Generic((0, VALUE), \\\n    " +
        generics.join(', \\\n    ') +
        " \\\n)(VALUE, EXPECTED, MESSAGE);\n" +
        decls.join(';') + ";";

    file_content = substitute_text(
        file_content, "#define ATT_STRING_AS_POINTERS 0\n#endif\n",
        "\n\nunsigned int att_get_valid_tests(void);",
        substitution);

    writeFileSync('./attractor.h', file_content);
}
