<xsl:template match="customhtml">
    <div id="{@id}" />
    <script>
         ui_fill_custom_html('<xsl:value-of select="@id"/>', '<xsl:value-of select="x:b64(@html)"/>');
    </script> 
</xsl:template>

<xsl:template match="fulldialog">
    <div id="{@id}" style="background-color: rgba(119, 119, 119,0.7); cursor: pointer; height: 100%; width: 100%; top:0; left:0; display: block; position: fixed">
            <div style="margin: {x:iif(@height or @width,'5% auto auto', '20px')}; height: {x:iif(not(@height) and @width, '80%', '')} {@height} {x:iif(not(@height) and not(@width),'95%', '')}; {x:iif(@width or @height,'width:','')} {x:iif(@height and not(@width), '95%', '')} {@width};">
        <div style="display: block;">
                <a style="background: url('/dl/core/ui/stock/close.png') transparent;cursor: pointer;height: 30px;position: absolute;width: 30px;z-index: 1103; margin-left: -13px; margin-top: -11px;" onclick="javascript:$(this).parent().parent().parent().hide();"></a>
            </div>
        <div style="display: block; background-color: rgb(255,255,255); opacity: 1.0; height: 100%;">
            <xsl:apply-templates />
        </div>
    </div>
    </div>
</xsl:template>

<xsl:template match="js">
    <script>
        <xsl:value-of select="@code"/>
    </script> 
</xsl:template>

<xsl:template match="null">
</xsl:template>

<xsl:template match="headtitle">
    <title><xsl:value-of select="@text"/></title>
</xsl:template>

<xsl:template match="completerefresh">
    <script>document.location.href="/";</script>
</xsl:template>

<xsl:template match="container">
    <div style="width: {x:css(@width, 'auto')}; height: {x:css(@height, 'auto')};">
           <xsl:apply-templates />
    </div>
</xsl:template>

<xsl:template match="pad">
    <div class="ui-el-pad">
           <xsl:apply-templates />
    </div>
</xsl:template>

<xsl:template match="hcontainer">
    <table class="layout" style="display: inline-block; width: {x:css(@width, 'auto')}; height: {x:css(@height, 'auto')};">
         <tr>
             <xsl:for-each select="*">
                <td style="padding-right: {x:css(../@spacing, '0')}">
                    <xsl:apply-templates select="." />
                </td>
             </xsl:for-each>    
         </tr>
    </table>
</xsl:template>

<xsl:template match="vcontainer">
     <table class="layout"  style="width: {x:css(@width, 'auto')}; height: {x:css(@height, 'auto')};">
         <xsl:for-each select="*">
             <tr><td style="padding-bottom: {x:css(../@spacing, '0')}">  
                 <xsl:apply-templates select="."/>
             </td></tr>
         </xsl:for-each>    
    </table>
</xsl:template>


<xsl:template match="spacer">
    <div style="width: {x:css(@width, '1')}; height: {x:css(@height, '1')};" />
</xsl:template>


<xsl:template match="scrollcontainer">
    <div class="ui-el-scrollcontainer" style="width: {x:css(@width, '200')}; height: {x:css(@height, '200')}; {x:iif(@noborder, 'border: none', '')}; {x:iif(@hide, 'display: none', '')}">
        <xsl:apply-templates />
    </div>
</xsl:template>
