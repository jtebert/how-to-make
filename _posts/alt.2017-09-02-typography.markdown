---
layout: post
title:  "Typography"
week: Week 0

---

Here are the different features and styles available for posts written in Markdown. To see how to create them, check out the source markdown file.

<!--more-->

# Headings

# Heading 1

## Heading 2

### Heading 3

#### Heading 4

##### Heading 5

###### Heading 6

# Media

## Images

Embedded image:

![Test image](https://i.pinimg.com/736x/1b/3c/1d/1b3c1d5522dd21a3d788c45c0f5de479.jpg)

Expandable image:

![Cat](https://static.pexels.com/photos/416160/pexels-photo-416160.jpeg){: .materialboxed}

Artisan VHS squid, austin excepteur slow-carb put a bird on it brunch enim occaecat adaptogen coloring book. Green juice sunt pickled +1 four loko before they sold out excepteur tote bag chartreuse banh mi sustainable man bun. Sint iPhone thundercats, tilde shabby chic aliqua actually pariatur copper mug minim cliche sriracha ullamco reprehenderit. Everyday carry vexillologist vinyl ad. Chia photo booth mustache, marfa woke pabst aliquip. Venmo bicycle rights prism, iceland cold-pressed mixtape brunch disrupt poke master cleanse velit succulents cred ad gastropub. Kombucha normcore bicycle rights excepteur art party. Drinking vinegar pork belly voluptate shoreditch kitsch, succulents id bespoke dolor lomo commodo. Stumptown hashtag tote bag keytar esse heirloom. Lomo fingerstache occupy ut, trust fund prism aliqua drinking vinegar excepteur wayfarers. Ugh biodiesel air plant, neutra squid waistcoat nostrud sartorial aliquip 90's.

![Code](https://static.pexels.com/photos/97077/pexels-photo-97077.jpeg){: .small .materialboxed}

Try-hard velit cardigan enim lyft sustainable, schlitz tilde listicle kickstarter. Direct trade umami elit wayfarers street art aliquip echo park eiusmod palo santo banjo copper mug artisan vice meditation. Keytar mustache drinking vinegar aliquip art party do activated charcoal migas nisi glossier neutra reprehenderit kitsch tilde. Tumeric brunch hexagon, single-origin coffee cray knausgaard health goth kombucha excepteur squid hot chicken vinyl id. Culpa distillery salvia crucifix glossier non cloud bread single-origin coffee meh brooklyn jean shorts palo santo. Commodo ramps skateboard, blog four dollar toast humblebrag sint mumblecore chambray aute plaid activated charcoal freegan neutra ut. Post-ironic gentrify distillery hell of raclette schlitz unicorn prism four dollar toast photo booth skateboard. Cliche ennui man bun, slow-carb mustache messenger bag paleo tbh meggings single-origin coffee. VHS tilde lomo, sunt poutine brooklyn tote. Etsy labore four dollar toast church-key. VHS next level cliche artisan post-ironic whatever keytar cred. Pariatur meditation chia letterpress elit tbh. Adaptogen palo santo hot chicken literally. Artisan scenester gentrify, in echo park listicle kickstarter asymmetrical jean shorts. Irure umami authentic, bitters photo booth kitsch vexillologist in typewriter ex occaecat 8-bit direct trade. Church-key in raclette, vice minim do cloud bread banjo hashtag. Slow-carb incididunt sartorial, messenger bag deep v post-ironic +1 biodiesel quinoa reprehenderit PBR&B consectetur kogi green juice. Venmo meditation lomo selvage, everyday carry distillery four dollar toast velit cray kinfolk vice. Marfa culpa gastropub eiusmod, PBR&B exercitation drinking vinegar four loko ut ex green juice. Everyday carry letterpress meggings 8-bit typewriter ullamco. Man braid veniam in truffaut kogi blue bottle fam ipsum. 

![Code](https://unsplash.it/200/400){: .small}
![Code](https://static.pexels.com/photos/97077/pexels-photo-97077.jpeg){: .small}

## Embedded Video

<iframe src="https://www.youtube.com/embed/69fr5bNiEfc" frameborder="0" allowfullscreen></iframe>

# Tables

**Markdown Extra** has a special syntax for tables:

Item      | Value
--------- | -----
Computer  | 1600 USD
Phone     | 12 USD
Pipe      | 1 USD

You can specify column alignment with one or two colons:

| Item      |    Value | Qty  |
| :-------- | --------:| :--: |
| Computer  | 1600 USD |  5   |
| Phone     |   12 USD |  12  |
| Pipe      |    1 USD | 234  |

# Lists

- Item 1
- Item 2
  - Subitem 2.1
  - Subitem 2.2
- Item 3

1. Item 1
2. Item 2
  - Subitem 2.1
  - Subitem 2.2
3. Item 3

# Blockquotes

You can write block quotes.

> This is a block quote.
> It's split over multiple lines.
>
> It can also have multiple paragraphs.

# Math (with MathJax)

You can render *LaTeX* mathematical expressions using **MathJax**, as on [math.stackexchange.com][1]:

There are inline equations like \\( y = x + 2 \\) indicated by `\\( ... \\)`.

You can also display an equation (not inline displayed) using inline code, like \\[ y = x + 4\\] which you do with `\\[ ... \\]`.

Or you can display a block of math by surrounding it with `$$ ... $$`. Like this:

$$
\Gamma(z) = \int_0^\infty t^{z-1}e^{-t}dt\,.
$$

> **Tip:** Make sure you include MathJax into your publications to render mathematical expression correctly. Your page/template should include something like: 

```
<script type="text/javascript" src="https://stackedit.io/libs/MathJax/MathJax.js?config=TeX-AMS_HTML"></script>
```

# Code

## Inline code

Code embedded in a paragraph looks like `this is code`. Code can also be in blocks with or without line numbers.

## Code blocks

Code blocks have syntax highlighting courtesy of highlight.js. You can specify the code language and whether you want the code block to include line numbers. (You can include line numbers using just `linenos` instead of `linenos=table`, but this puts the line numbers in the same lines as the code so that it becomes impossible to copy and paste.)

Without line numbers:

{% highlight ruby %}
  def print_hi(name)
    puts "Hi, #{name}"
  end
  print_hi('Tom')
  #=> prints 'Hi, Tom' to STDOUT. This is a long line to test scrolling if it goes past the box.
{% endhighlight %}

With line numbers:

{% highlight ruby linenos %}
  def print_hi(name)
    puts "Hi, #{name}"
  end
  print_hi('Tom')
  #=> prints 'Hi, Tom' to STDOUT.
{% endhighlight %}


# Definition Lists

**Markdown Extra** has a special syntax for definition lists too:

Term 1
Term 2
:   Definition A
:   Definition B

Term 3

:   Definition C

:   Definition D

	> part of definition D

# Dividers

Divide up sections and stuff with dividers.

---

They look like that.

# Font weights and styles

This text is **bold text**.

This is *italicized* text.

# Links and Footnotes

You can create footnotes like this[^footnote].

  [^footnote]: Here is the *text* of the **footnote**.

You can embed links with a direct link, like [this link to Google](http://www.google.com). Links can also be in a separated form like [this link to the Jekyll documention][jekyll]. (That's useful if you want to link to the same website multiple times [like this][jekyll].)

[1]: http://math.stackexchange.com/
[jekyll]:      http://jekyllrb.com

---
