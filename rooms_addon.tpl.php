<div class="rooms-addon clearfix">
  <div class="rooms-addon-thumbnail"><img src="<?php print $thumbnail; ?>" /></div>
  <div class="rooms-addon-info">
    <h2><?php print $name; ?></h2>
    <?php print $description; ?>
    <div class="rooms-addon-version-info">
      Latest Version: <?php print $latest_version; ?>
      <?php if ($installed_version): ?>
        Installed Version: <?php print $installed_version; ?>
      <?php endif; ?>
    </div>
    <div class="rooms-addon-license-info">
      <?php if ($installed_version): ?>
        License Key: <span class="rooms-addon-license-key"><?php print $license_key; ?></span>
      <?php endif; ?>
      <?php if ($installed_version && $update_available): ?>
        <?php print $updates_link; ?>
      <?php else: ?>
        <?php print $purchase_link; ?>
      <?php endif; ?>
    </div>
  </div>
</div>
