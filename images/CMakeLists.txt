add_custom_target(gen_image_include
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  COMMAND convert free.svg free.png
)
