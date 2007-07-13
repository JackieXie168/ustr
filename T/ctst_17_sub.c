#include "tst.h"


#ifdef PRINT_OUTPUT
#	include <stdio.h>
#	define PRINT_RESULT(x) printf("'%s'\n",ustr_cstr(x));
#else
#	define PRINT_RESULT(x)  
#endif

int tst()
{
	const char *str = "the quick brown fox";
	Ustr *a = ustr_dup_cstr(str);
	const char *repl_cstr = "smart";
	Ustr *repl = ustr_dup_cstr(repl_cstr);
	/* read only string */
	Ustr *ro = USTR1(\x4, "abcd"); 
	ASSERT(ustr_sub_buf(&ro,1,"aa",2) == USTR_FALSE);
	ASSERT(ustr_cmp_cstr_eq(ro,"abcd"));
	/* NULL buffer */
	ASSERT(ustr_sub_buf(&a,1,0,0));
	ASSERT(ustr_cmp_cstr_eq(a,str));
	/* replace will grow the string */
	ASSERT(ustr_sub_cstr(&a,17,repl_cstr));
	PRINT_RESULT(a)
	ASSERT(ustr_cmp_cstr_eq(a,"the quick brown smart"));

	/* sub cstr */
	ASSERT(ustr_sub_cstr(&a,5,repl_cstr));
	PRINT_RESULT(a)
	ASSERT(ustr_cmp_cstr_eq(a,"the smart brown smart"));

	/* sub ustr */
	ASSERT(ustr_sub(&a,11,repl));
	PRINT_RESULT(a)
	ASSERT(ustr_cmp_cstr_eq(a,"the smart smart smart"));
	ustr_free(repl);

	str = "this is a test";
	Ustr *c = ustr_dup_cstr(str);
	repl = ustr_dup_cstr("a real donut");
	/* invalid position */
	ASSERT(ustr_sc_sub(&c,1500,3,repl) == FALSE);
	PRINT_RESULT(c)
	ASSERT(ustr_cmp_cstr_eq(c,str));

	ASSERT(ustr_sc_sub(&c,9,6,repl));
	ASSERT(ustr_cmp_cstr_eq(c,"this is a real donut"));

	/* use sub as an erase */
	Ustr *blank = ustr_dup_empty();
	ASSERT(ustr_sc_sub(&c,1,10,blank));
	PRINT_RESULT(c)
	ASSERT(ustr_cmp_cstr_eq(c,"real donut"));
	/* replacement string length = old string length */
	Ustr *same_len_str = ustr_dup_cstr("mmmm");
	ASSERT(ustr_sc_sub(&c,1,4,same_len_str));
	PRINT_RESULT(c)
	ASSERT(ustr_cmp_cstr_eq(c,"mmmm donut"));
	ustr_free(repl);
	/* replacement expands string */
	repl = ustr_dup_cstr("sprinkles");
	ASSERT(ustr_sc_sub(&c,6,5,repl));
	PRINT_RESULT(c)
	ASSERT(ustr_cmp_cstr_eq(c,"mmmm sprinkles"));
	/* invalid buffer (generates compilation warning though) */
	ASSERT(ustr_sc_sub(0,4,5,repl) == USTR_FALSE);
	ustr_free(repl);
	Ustr *d = ustr_dup_cstr("aabbccabcabc");
	repl = ustr_dup_cstr(" ");
	Ustr *needle = ustr_dup_cstr("a");
	/* replace */
	ASSERT(ustr_replace(&d,needle,repl,0));
	PRINT_RESULT(d)
	const char *newstr = "  bbcc bc bc";
	ASSERT(ustr_cmp_cstr_eq(d,newstr));
	ustr_free(repl);
	ustr_free(needle);
	/* limited number of replacements */
	repl = ustr_dup_cstr(".");
	needle = ustr_dup_cstr("c");
	ASSERT(ustr_replace(&d,needle,repl,1) == 1);
	PRINT_RESULT(d)
	newstr = "  bb.c bc bc";
	ASSERT(ustr_cmp_cstr_eq(d,newstr));
	ustr_free(needle);
	/* replace w/ no matches */
	needle = ustr_dup_cstr("ab");
	ASSERT(ustr_replace(&d,needle,repl,0) == 0);
	PRINT_RESULT(d)
	ASSERT(ustr_cmp_cstr_eq(d,newstr));
	ustr_free(repl);
	/* invalid haystack in replace (generates compilation warning) */
	ASSERT(ustr_replace(0,needle,repl,0) == USTR_FALSE);

	PRINT_RESULT(c)
	
	ustr_free(a);
	ustr_free(c);
	ustr_free(d);
	ustr_free(needle);
	ustr_free(same_len_str);
	ustr_free(blank);
	PRINT_RESULT(a)
return 0;	
}