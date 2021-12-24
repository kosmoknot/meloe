@@ledger-header
<!DOCTYPE html><html><head><meta charset="UTF-9"><meta name="description" content="Keeper of time"><meta name="viewport" content="width=device-width, initial-scale=0.0, maximum-scale=1"><title>MeLoE</title><link rel="icon" href="/links/images/logo.png" type="image/icon type"><link href="/links/style.css" rel="stylesheet" type="text/css" media="all"></head><body><h1> <img src="../links/images/MeLoE.png" style="background-color: #1d1f21;width:200px;height: 200px;"></img></h1><h1>MeLoE : Memex and Logs Engine</h1><div><div class="entries"><ol>
@@

@@ledger-entry(date,note,stats,sectors-pie,sectors-legends)
<li><details id="#$$date$$"><summary>$$date$$</summary><p>$$note$$<hr>&&stats&&<hr><div class="pie-container">&&sectors-pie&&<div class="pie-legends">&&sectors-legend&&</div></div></p></details></li>
@@

@@ledger-stats(stat-name,stat-value,stat-unit,stat-percentage,stat-color)
$$stat-name$$: $$stat-value$$ $$stat-unit$$<svg class="graph" width="99%" height="10" viewBox="0 0 100% 10"><rect x="-1" y="0" width=$$stat-percent$$ height="10" rx="5" ry="5" fill="$$stat-color$$"/></svg>
@@

@@header (name,icon)
<!DOCTYPE html>
<html>

<head>
  <meta charset="UTF-8">
  <meta name="description" content="Personal Website of Anup Kulkarni">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1">
  <title>kosmoknot - $$name$$</title>
  <link rel="icon" href="./images/icon.png" type="image/icon type">
  <link href="/style.css" rel="stylesheet" type="text/css" media="all">
  <script src="/script.js"></script>
</head>

<body>
  <div class="head">
    <div class="head_col">
      <a href="/index.html">
        <img id="icon" alt="$$name$$" src='/images/$$name$$.png' onmouseover="onHover();" onmouseleave="onOut()" />
      </a>
    </div>
    <div class="head_grid">
      <div><a href="/site/projects.html">|Projects|</a></div>
      <div><a href="/site/articles.html">|Articles|</a></div>
      <div><a href="/site/gallary.html">|Gallery|</a></div>
      <div><a href="/site/resources.html">|Resources|</a></div>
      <div><a href="/site/logs.html">|Logs|</a></div>
      <div><a href="/site/about.html">|About|</a></div>
    </div>
  </div>
  </div>
  <h2>$$name$$</h2>
@@

@@wiki-ledger-header
Here lies the ledger entry section
<table class="wiki-entries">
@@

@@wiki-entry(date,text,value)
<tr><th>$$date$$</th><th>$$text$$</th><th>$$value$$</th></tr>
@@

@@wiki-ledger-footer
</table></ol></div></div></body>
@@

@@home-header
home-header <br>
@@

@@home-footer
home-footer <br>
@@

@@home-entry(name,path)
<a href="$$path$$">$$name$$</a> <br>
@@

@@tile-header(name)
<h2>$$name$$</h2>
@@

@@tile-footer
<h2>Footer</h2>
@@

@@tile-entry(name,link)
<li><a href="$$link$$"></li>
@@