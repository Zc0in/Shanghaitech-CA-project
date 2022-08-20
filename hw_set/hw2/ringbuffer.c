#include"ringbuffer.h"
/* Create a new ring buffer. */
ring_buffer* ring_buffer_new(){
   ring_buffer* buffer;
   /* Allocate memory for buffer */
   buffer = (ring_buffer*)malloc(sizeof(ring_buffer));
   /* Initialize*/
   buffer->capacity = RING_BUFFER_INIT_SIZE;
   buffer->read_pos = 0;
   buffer->write_pos = 0;
   /* Allocate memory for buffer->content */
   buffer->content = (int*)malloc(buffer->capacity * sizeof(int));
   return buffer;
}
/* Destroy the given ring buffer. */
void ring_buffer_destroy(ring_buffer** buffer){
   /* Free */
   free((*buffer)->content);
   free((*buffer));
   *buffer = NULL;
}
/* Returns true if the given ring buffer is empty, false otherwise */
bool ring_buffer_is_empty(const ring_buffer* buffer){
   /* Judge whather it is empty*/
   if (buffer != NULL && buffer->read_pos == buffer->write_pos){
      return true;
   }
   /* False otherwise */
   return false;
}
/* Read an element from the ring buffer. */
bool ring_buffer_read(ring_buffer* buffer, int* data){
   if (buffer!= NULL && data != NULL && !ring_buffer_is_empty(buffer)){
      /* Read */
      *data = buffer->content[buffer->read_pos];
      /* Update read_pos */
      buffer->read_pos = (buffer->read_pos + 1) % buffer->capacity;
      return true;
   }
   /* False otherwise */
   return false;
}
/* Write an element which value is `data` to the ring buffer. */
bool ring_buffer_write(ring_buffer* buffer, const int data){
   /* Enough space to write */
   if (buffer != NULL && (buffer->write_pos + 1) % buffer->capacity != buffer->read_pos){
      buffer->content[buffer->write_pos] = data;
      /* Update write_pos */
      buffer->write_pos = (buffer->write_pos + 1) % buffer->capacity;
      return true;
   }
   /* The space is full */
   else if (buffer != NULL){
      int* temp;
      size_t i, j;
      /* Create a temp array */
      temp = (int*)malloc((buffer->capacity - 1) * sizeof(int));
      j = buffer->read_pos;
      /* Rearrange the content */
      for (i = 0; i < buffer->capacity - 1; i++){
         temp[i] = buffer->content[j];
         j = (j + 1) % buffer->capacity;
      }
      /* Copy back */
      for (i = 0; i < buffer->capacity - 1; i++){
         buffer->content[i] = temp[i];
      }
      /* Free temp */
      free(temp);
      /* Update two position */
      buffer->read_pos = 0;
      buffer->write_pos = buffer->capacity - 1;
      /* Increase the size of buffer */
      buffer->capacity = buffer->capacity < 1024 ? buffer->capacity * RING_BUFFER_GROW_FACTOR1 : buffer->capacity * RING_BUFFER_GROW_FACTOR2;
      buffer->content = (int*)realloc(buffer->content, buffer->capacity * sizeof(int));
      /* Fail to reallocate memory */
      if (buffer->content == NULL){
         return false;
      }
      /* Write */
      buffer->content[buffer->write_pos] = data;
      buffer->write_pos = (buffer->write_pos + 1) % buffer->capacity;
      return true;
   }
   /* False otherwise */
   return false;
}
/* Read "rdsize" elements from the ring buffer, and fill them to "data". */
bool ring_buffer_read_multi(ring_buffer* buffer, size_t rdsize, int* data){
   size_t space = 0, i = 0;
   /* Calculate the spare space */
   if (buffer != NULL){
      space = buffer->write_pos > buffer->read_pos ? buffer->write_pos - buffer->read_pos : buffer->capacity + buffer->write_pos - buffer->read_pos;
   }
   /* False otherwise */
   else{
      return false;
   }
   /* Judge */
   if (data != NULL && space >= rdsize){
      for (i = 0; i < rdsize; i++){
         /* Read */
         ring_buffer_read(buffer, &data[i]);
      }
      /* True */
      return true;
   }
   /* False otherwise */
   return false;
}
/* Write "wrtsize" elements to the ring buffer. */
bool ring_buffer_write_multi(ring_buffer* buffer, size_t wrtsize, const int* data){
   size_t i = 0;
   /* Judge */
   if (buffer != NULL && data != NULL){
      for (i = 0; i < wrtsize; i++){
         /* Write */
         ring_buffer_write(buffer, data[i]);
      }
      /* True */
      return true;
   }
   /* False otherwise */
   return false;
}
/* For every element in the ring buffer, apply "func" to it. */
bool ring_buffer_map(ring_buffer* buffer, map_func func){
   size_t j = buffer->read_pos;
   /* Judge */
   if (buffer != NULL){
      for (j = 0; j < buffer->capacity; j++){
         /* Apply func to elements */
         buffer->content[j] = func(buffer->content[j]);
      }
      return true;
   }
   /* False otherwise */
   return false;
}