/* Copyright (c) 2007 James Antill -- See LICENSE file for terms. */

#ifndef USTR_SRCH_H
#error " Include ustr-srch.h before this file."
#endif

#ifndef USTR_CONF_HAVE_MEMRCHR /* GNU extension */
#ifdef __GLIBC__
#define USTR_CONF_HAVE_MEMRCHR 1
#else
#define USTR_CONF_HAVE_MEMRCHR 0
#endif
#endif

USTR_CONF_I_PROTO size_t ustr_srch_chr_fwd(const struct Ustr *s1, size_t off,
                                           char val)
{
  const char *ptr = ustr_cstr(s1);
  size_t len = ustr_len(s1);
  const char *tmp;

  USTR_ASSERT(ustr_assert_valid(s1));

  USTR_ASSERT_RET(off <= len, 0);
  ptr += off;
  len -= off;

  if (!(tmp = memchr(ptr, val, len))) return (0);

  len = tmp - ptr;
  return (len + 1);
}

#if USTR_CONF_HAVE_MEMRCHR /* GNU extension */
USTR_CONF_I_PROTO size_t ustr_srch_chr_rev(const struct Ustr *s1, size_t off,
                                           char val)
{
  const char *ptr = ustr_cstr(s1);
  size_t len = ustr_len(s1);
  const char *tmp;

  USTR_ASSERT(ustr_assert_valid(s1));
  
  USTR_ASSERT_RET(off <= len, 0);
  len -= off;

  if (!(tmp = memrchr(ptr, val, len))) return (0);

  len = tmp - ptr;
  return (len + 1);
}
#else
USTR_CONF_I_PROTO size_t ustr_srch_chr_rev(const struct Ustr *s1, size_t off,
                                           char val)
{ /* slow... */
  const char *ptr = ustr_cstr(s1);
  size_t len = ustr_len(s1);
  const char *tmp = ptr;
  const char *prev = 0;

  USTR_ASSERT(ustr_assert_valid(s1));
  
  USTR_ASSERT_RET(off <= len, 0);
  len -= off;

  while ((tmp = memchr(tmp, val, len - (tmp - ptr))))
  {
    prev = tmp;
    ++tmp;
  }
  
  if (!prev)
    return (0);

  len = prev - ptr;
  return (len + 1);
}
#endif

#if ! USTR_CONF_HAVE_MEMMEM
USTR_CONF_i_PROTO void *ustr__sys_memmem(const void *hs, size_t hslen,
                                         const void *nd, size_t ndlen)
{
  const char *ptr;
  const char *end;

  if (ndlen == 0)
    return ((void *)hs);

  if (hslen < ndlen)
    return (0);

  end = ((const char *)hs) + hslen - ndlen;
  for (ptr = hs; ptr <= end; ++ptr)
    if (!memcmp(ptr, nd, ndlen))
      return ((void *)ptr);

  return (0);
}
#endif

USTR_CONF_I_PROTO size_t ustr_srch_buf_fwd(const struct Ustr *s1, size_t off,
                                           const void *val, size_t vlen)
{
  const char *ptr = ustr_cstr(s1);
  size_t len = ustr_len(s1);
  char *tmp = 0;

  USTR_ASSERT(ustr_assert_valid(s1));
  
  if (vlen == 1)
    return (ustr_srch_chr_fwd(s1, off, ((const char *)val)[0]));

  USTR_ASSERT_RET(off <= len, 0);
  ptr += off;
  len -= off;

  if (!(tmp = USTR__SYS_MEMMEM(ptr, len, val, vlen)))
    return (0);

  len = tmp - ptr;
  return (len + 1);
}

USTR_CONF_I_PROTO size_t ustr_srch_buf_rev(const struct Ustr *s1, size_t off,
                                           const void *val, size_t vlen)
{
  const char *ptr = ustr_cstr(s1);
  size_t len = ustr_len(s1);
  const char *prev = 0;
  const char *tmp  = 0;

  USTR_ASSERT(ustr_assert_valid(s1));
  
  if (vlen == 0)
    return (len ? len : 1);
  if (vlen == 1)
    return (ustr_srch_chr_rev(s1, off, ((const char *)val)[0]));

  USTR_ASSERT_RET(off <= len, 0);
  len -= off;

  tmp = ptr;
  while (((len - (tmp - ptr)) >= vlen) &&
         (tmp = USTR__SYS_MEMMEM(tmp, len - (tmp - ptr), val, vlen)))
  {
    prev = tmp;
    ++tmp;
  }
  if (!prev)
    return (0);

  len = prev - ptr;
  return (len + 1);
}

USTR_CONF_I_PROTO
size_t ustr_srch_subustr_fwd(const struct Ustr *s1, size_t off,
                             const struct Ustr *s2, size_t pos, size_t len)
{
  USTR_ASSERT(ustr_assert_valid(s1) && ustr_assert_valid(s2));
  
  if (!ustr__valid_subustr(s2, pos, len))
    return (ustr_srch_buf_fwd(s1, off, "", 0));
  
  return (ustr_srch_buf_fwd(s1, off, ustr_cstr(s2) + --pos, len));
}

USTR_CONF_I_PROTO
size_t ustr_srch_subustr_rev(const struct Ustr *s1, size_t off,
                             const struct Ustr *s2, size_t pos, size_t len)
{
  USTR_ASSERT(ustr_assert_valid(s1) && ustr_assert_valid(s2));
  
  if (!ustr__valid_subustr(s2, pos, len))
    return (ustr_srch_buf_rev(s1, off, "", 0));
  
  return (ustr_srch_buf_rev(s1, off, ustr_cstr(s2) + --pos, len));  
}
