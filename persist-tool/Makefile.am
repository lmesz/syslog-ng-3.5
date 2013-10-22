
persist_tool_persist_tool_CPPFLAGS=      \
	$(AM_CPPFLAGS)

bin_PROGRAMS += persist-tool/persist-tool

persist_tool_persist_tool_SOURCES = \
	persist-tool/persist-tool.c	\
	persist-tool/persist-tool.h 	\
	persist-tool/dump.c		\
	persist-tool/dump.h		\
	persist-tool/add.c		\
	persist-tool/add.h		\
	persist-tool/generate.c		\
	persist-tool/generate.h		\
	persist-tool/generate-agent.c	\
	persist-tool/generate-agent.h

persist_tool_persist_tool_LDADD =  $(SYSLOG_NG_LIBS) -ljson
