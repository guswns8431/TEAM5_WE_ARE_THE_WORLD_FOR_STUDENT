#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

#include "./Utilizer.hpp"

enum request_progress {
  on_continuous,
  on_finish
};

enum request_status {
  on_request_line,
  on_headers,
  on_validating_headers,
  on_body,
  on_chunk,
  on_complete,
  on_error
};

enum chunk_status {
  on_chunk_size,
  on_chunk_body
};

class Request {
 private:
  std::string _data;
  Headers _headers;
  std::string _method;
  std::string _body;
  std::string _query_string;
  std::string _target;
  std::string _protocol;

  std::size_t _chunk_size;
  std::size_t _body_offset;
  std::size_t _content_length;

  struct timeval _header_timer;
  struct timeval _body_timer;

  chunk_status _chunk_status;
  request_status _request_status;

  int parse_request_line(void);
  int parse_headers(void);
  int validate_headers(void);
  int parse_body(void);
  int validate_chunk_trailer(void);
  int parse_chunk(void);

  void check_error_and_set_request_status(int code);
  void case_on_request_line(int *code);
  void case_on_headers(int *code);
  void case_on_validating_headers(int *code);
  void case_on_body(int *code);
  void case_on_chunk_size(std::size_t crlf_position);
  void case_on_chunk_body(std::size_t crlf_position);
  void case_on_chunk(int *code);

  void remove_current_word_from_data(std::size_t length);
  void remove_crlf_from_data(std::size_t crlf_position);
  void remove_query_string_from_target(void);
  std::string get_current_word_from_data(void) const;
  std::string get_query_string_from_target(void) const;
  std::size_t get_crlf_position_from_data(void) const;
  std::size_t get_blank_position_from_data(void) const;
  std::size_t get_colon_position_from_data(void) const;
  std::size_t get_question_position_from_target(void) const;

  bool is_method_GET(void) const;
  bool is_method_POST(void) const;
  bool is_method_HEAD(void) const;
  bool is_method_PUT(void) const;
  bool is_method_DELETE(void) const;
  bool is_valid_method(const std::string& method) const;
  bool is_valid_target(void) const;
  bool is_valid_pair_on_colon_separated(void) const;
  bool is_valid_host_on_header(void) const;
  bool is_chunk_transfer(void) const;
  bool is_body_transfer(void) const;
  bool is_target_begin_with_separator(void) const;
  bool is_target_length_too_long(void) const;
  bool is_target_queried(void) const;
  bool is_header_too_long(const std::string& key,
                          const std::string& val) const;
  bool is_data_separatable(void) const;
  bool is_host_duplicated(const std::string& key) const;
  bool is_request_status_completable(int code) const;
  bool is_body_ready_to_be_sent(void) const;
  bool is_on_request_line(void) const;
  bool is_on_headers(void) const;
  bool is_on_validating_headers(void) const;
  bool is_on_body(void) const;
  bool is_on_chunk(void) const;
  bool is_on_complete(void) const;
  bool is_on_error(void) const;
  bool is_on_chunk_size(void) const;
  bool is_on_chunk_body(void) const;
  bool is_chunk_size_empty(void) const;

  Request(const Request& r);
  Request& operator=(const Request& r);

 public:
  Request(void);
  ~Request(void);

  bool is_header_validated(void) const;
  bool is_timeout(void);

  int parse(const std::string &data);

  time_t get_header_time(void) const;
  time_t get_body_time(void) const;

  friend class ReqContext;
};

#endif
