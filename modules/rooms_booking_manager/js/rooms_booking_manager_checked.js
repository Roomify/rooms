(function ($) {
  $(document).ready(function() {
    $('#edit-perc-textfield').focus(function()
    {
      $('input[name=rooms_payment_options][value=11]').attr('checked', true);
    });
  });
})(jQuery);
