# Description

In this file I decided to put some points which describe my decisions/findings. Just for info and maybe it will be usable for me in longer term to learn from mistakes.

## Background.

I'm not a c / c++ developer, so many parts of coda can be done better. Also decisions which I done was dictated by my java experience, so it might look odd for c++ guys. Also my code style come from java, you have to live with this.

# Goal

* Code which do what I need from functional point of view. This implies:
  * I don't care if this can be done better. This can be applied any time later when I've working base. Yes, I know this increase technical debt, and yes I don't care.
  * I don't care about heavy refactors to make whole code consistent at all time. This can be done later.
* I'll try to document public api (REST API) so it should be most time up to date. However this is hard ;(

# Current tech debts (which I'm aware now)

* Some parts of code (especially storage) is inconsistent. It's because of my lack of c++ knowledge, which forced me to heavy refactors to search acceptable solution.

* I still struggle between raw pointers and smart pointers and copies of objects. You can notice it in many places.

* Libblepp is in very poor state, but it has minimum functionality which I need. So I wrapped this and currently using as a much easier/stable then previous abnomination around libgatt.

# Breaking decisions

1. C++ is not Java... Especially templates/generics. This hit me heavy when working on Storage and serializers interface. I believe this need to be revisited but at this moment (30-12-2016) I don't know what is best approach here.
2. In the beginning of work I decided to use sqlite as a storage backend. In many aspects it's good, but this brings a lot of pain... There is no automated ORM for C++ which is acceptable for me. A lot of code/time was wasted to achieve very limited effects. But in reality I don't need this at all. It's reasonable only to use it for storing measurements. But for all other things like rooms/sensors it's overkill which doesn't bring any value. So at 30-12-2016 I decided to not use it at all. Wipe those things in current development phase and just put file storage. If you think about what you want to have from functional point of view, with minimal work needed this is optimal solution. Probably will be reworked in future if some new use-cases appears.
