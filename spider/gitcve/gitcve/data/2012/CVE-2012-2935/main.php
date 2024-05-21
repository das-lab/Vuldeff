<?php
/**
 * osCommerce Online Merchant
 * 
 * @copyright Copyright (c) 2011 osCommerce; http://www.oscommerce.com
 * @license BSD License; http://www.oscommerce.com/bsdlicense.txt
 */

  use osCommerce\OM\Core\HTML;
  use osCommerce\OM\Core\OSCOM;
?>

<h1><?php echo $OSCOM_Template->getPageTitle(); ?></h1>

<?php
  if ( $OSCOM_MessageStack->exists('Cart') ) {
    echo $OSCOM_MessageStack->get('Cart');
  }
?>

<div class="moduleBox">
  <h6><?php echo OSCOM::getDef('shopping_cart_heading'); ?></h6>

  <form name="shopping_cart" action="<?php echo OSCOM::getLink(null, null, 'Update', 'SSL'); ?>" method="post">

  <div class="content">
    <table border="0" width="100%" cellspacing="0" cellpadding="2">

<?php
    $_cart_date_added = null;

    foreach ( $OSCOM_ShoppingCart->getProducts() as $products ) {
      if ( $products['date_added'] != $_cart_date_added ) {
        $_cart_date_added = $products['date_added'];
?>

      <tr>
        <td colspan="4"><?php echo sprintf(OSCOM::getDef('date_added_to_shopping_cart'), $products['date_added']); ?></td>
      </tr>

<?php
      }
?>

      <tr>
        <td valign="top" width="60"><?php echo HTML::button(array('href' => OSCOM::getLink(null, null, 'Delete=' . $products['item_id'], 'SSL'), 'icon' => 'trash', 'title' => OSCOM::getDef('button_delete'))); ?></td>
        <td valign="top">

<?php
      echo HTML::link(OSCOM::getLink(null, 'Products', $products['keyword']), '<b>' . $products['name'] . '</b>');

      if ( (STOCK_CHECK == '1') && ($OSCOM_ShoppingCart->isInStock($products['item_id']) === false) ) {
        echo '<span class="markProductOutOfStock">' . STOCK_MARK_PRODUCT_OUT_OF_STOCK . '</span>';
      }

// HPDL      echo '&nbsp;(Top Category)';

      if ( $OSCOM_ShoppingCart->isVariant($products['item_id']) ) {
        foreach ( $OSCOM_ShoppingCart->getVariant($products['item_id']) as $variant) {
          echo '<br />- ' . $variant['group_title'] . ': ' . $variant['value_title'];
        }
      }
?>

        </td>
        <td valign="top"><?php echo HTML::inputField('products[' . $products['item_id'] . ']', $products['quantity'], 'size="4"'); ?> <a href="#" onclick="document.shopping_cart.submit(); return false;">update</a></td>
        <td valign="top" align="right"><?php echo '<b>' . $OSCOM_Currencies->displayPrice($products['price'], $products['tax_class_id'], $products['quantity']) . '</b>'; ?></td>
      </tr>

<?php
    }
?>

    </table>
  </div>

  </form>

  <table border="0" width="100%" cellspacing="0" cellpadding="2">

<?php
// HPDL
//    if ($osC_OrderTotal->hasActive()) {
//      foreach ($osC_OrderTotal->getResult() as $module) {
      foreach ( $OSCOM_ShoppingCart->getOrderTotals() as $module ) {
        echo '    <tr>' . "\n" .
             '      <td align="right">' . $module['title'] . '</td>' . "\n" .
             '      <td align="right">' . $module['text'] . '</td>' . "\n" .
             '    </tr>';
      }
//    }
?>

  </table>

<?php
    if ( (STOCK_CHECK == '1') && ($OSCOM_ShoppingCart->hasStock() === false) ) {
      if ( STOCK_ALLOW_CHECKOUT == '1' ) {
        echo '<p class="stockWarning" align="center">' . sprintf(OSCOM::getDef('products_out_of_stock_checkout_possible'), STOCK_MARK_PRODUCT_OUT_OF_STOCK) . '</p>';
      } else {
        echo '<p class="stockWarning" align="center">' . sprintf(OSCOM::getDef('products_out_of_stock_checkout_not_possible'), STOCK_MARK_PRODUCT_OUT_OF_STOCK) . '</p>';
      }
    }
?>

</div>

<div class="moduleBox">
  <form name="checkout" action="<?php echo OSCOM::getLink(null, 'Checkout', null, 'SSL'); ?>" method="post">

  <div style="float: right;">
    <?php echo HTML::button(array('icon' => 'triangle-1-e', 'title' => OSCOM::getDef('button_checkout'))); ?>
  </div>

<?php
  if ( !$OSCOM_Customer->isLoggedOn() && $OSCOM_Application->requireCustomerAccount() ) {
?>

  <div class="content">
    <?php echo 'E-Mail Address: ' . HTML::inputField('email', $OSCOM_Customer->getEMailAddress()) . ' or ' . HTML::link(OSCOM::getLink(null, 'Account', 'LogIn', 'SSL'), 'Sign-In') . ' to process this order'; ?>
  </div>

<?php
  }
?>

  </form>
</div>
