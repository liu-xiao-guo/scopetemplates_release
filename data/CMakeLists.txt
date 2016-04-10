
# Install the scope images
install(
  FILES
    "icon.png"
    "logo.png"
    "screenshot.png"
  DESTINATION
    "${SCOPE_INSTALL_DIR}"
)

INSTALL(
    DIRECTORY "images"
    DESTINATION ${SCOPE_INSTALL_DIR}
)

