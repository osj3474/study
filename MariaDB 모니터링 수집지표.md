# Stat 지표

- Innodb_buffer_pool_read_requests
- Innodb_buffer_pool_reads
- Threads_connected
- Questions
- Com_select
- Writes
- Bytes_received
- Bytes_sent
- Innodb_row_lock_current_waits
- Connections
- Com_commit
- Com_rollback
- Innodb_rows_read
- Innodb_rows_writes
- Innodb_row_lock_waits
- Innodb_row_lock_time



# Active Session 지표

- id
- user
- host
- db
- command
- state
- os_id
- thread_id
- time
- type
- name
- connection_type
- query
- query_param



# Dead Lock 지표

- dead lock time
- dead lock id
- thread id
- query id
- host
- user
- state
- query



# Lock Tree 지표

- id (holder_id/waiter_id)
- \<user>
- \<db>
- \<host>
- \<time>
- holder_type
- lock mode (holder_mode)
- waiter_type
- lock request(waiter_mode)
- state (holder_table/waiter_table)
- table (holder_table/waiter_table)
- index(holder_index/waiter_index)
- \<query>