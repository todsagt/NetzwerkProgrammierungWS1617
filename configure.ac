AC_INIT(netzwerkprogrammierung_beispiele, 1.2)
AC_CONFIG_SRCDIR(echo_server_udp.c, echo_client_udp.c, Socket.c, tcp_client.c, daytime_server_tcp.c, discard_server_tcp.c, echo_server_tcp.c, chargen_server_tcp.c, server.c, client.c, helper.c, getaddresses.c, chargen.c, daytime.c, discard.c, echo.c)
AM_INIT_AUTOMAKE

AC_PROG_CC

AC_CANONICAL_HOST

CFLAGS="$CFLAGS -Wall -pedantic -pthread"

case $host_os in
solaris*)
  CFLAGS="$CFLAGS -D_XPG4_2"
  ;;
esac

AC_CHECK_TYPE(size_t)
AC_CHECK_TYPE(ssize_t)

AC_CHECK_FUNCS(socket, , [AC_CHECK_LIB(socket, socket)])
AC_CHECK_FUNCS(inet_addr, , [AC_CHECK_LIB(nsl, inet_addr)])

AC_CHECK_HEADERS(sys/types.h)
AC_CHECK_MEMBER(struct sockaddr_in.sin_len,
                AC_DEFINE(HAVE_SIN_LEN, 1, [Define this if your system has sin_len in sockaddr_in struct.]),,
                [#ifdef HAVE_SYS_TYPES_H
                 #include <sys/types.h>
                 #endif
                 #include <netinet/in.h>])

AC_MSG_CHECKING(for socklen_t)
AC_TRY_COMPILE([#ifdef HAVE_SYS_TYPES_H
                #include <sys/types.h>
                #endif
                #include <sys/socket.h>],
               [socklen_t x; x = 1; return 0;],
               [AC_MSG_RESULT(yes)],
               [AC_MSG_RESULT(int)
                AC_DEFINE(socklen_t, int, [Define a type for socklen_t.])]) 

AC_OUTPUT(Makefile)
