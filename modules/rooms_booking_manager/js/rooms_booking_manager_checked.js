(function ($) {
  $(document).ready(function() {
    var clearMePrevious = '';
    // clear input on focus
    $('#edit-perc-textfield').focus(function()
    {
        $('input[name=rooms_payment_options][value=11]').attr('checked', true);
    });
  });
})(jQuery);
;