(function ($) {
  /**
   * Booking unit options widget JS operations.
   */
  Drupal.behaviors.rooms_options_widget = {
    attach: function(context) {
      $('.rooms-option--remove-button').click(function(e) {
        $(this).parent().find('.rooms-option--name').val('');
        $(this).parents('tr').hide();
        e.preventDefault();
      });
    }
  };
})(jQuery);
