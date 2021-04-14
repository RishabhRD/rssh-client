add_executable(rssh-client
  MessageType.cc
  ListenerMessage.cc
  InvalidMessageCastException.cc
  CloseMessage.cc
  NewMessage.cc
  DataMessage.cc
  Message.cc
  MessageParsingException.cc
  main.cc
  )
target_include_directories(rssh-client PUBLIC ../include)
target_link_libraries(rssh-client PRIVATE Threads::Threads)
target_compile_features(rssh-client PUBLIC cxx_std_17)
install(TARGETS rssh-client RUNTIME DESTINATION bin)
