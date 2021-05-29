# MyVector

## Spartos tyrimas ##

Naudojami `int` tipo kintamieji. Įrašomi naudojant `push_back`.

Container | 10000 | 100000 | 1000000 | 10000000 | 100000000
------------ | -------------| -------------| -------------| -------------| -------------
*std::vector* | 0 ms | 1 ms | 15 ms | 140 ms | 1492 ms 
*Vector* | 0 ms | 1 ms | 16 ms | 194 ms | 1874 ms

## Atminties perskirstymai ##

Naudojami `int` tipo kintamieji. Įrašomi naudojant `push_back`.

Container | 10000 | 100000 | 1000000 | 10000000 | 100000000
------------ | -------------| -------------| -------------| -------------| -------------
*std::vector* | 15 | 18 | 21 | 25 | 28
*Vector* | 15 | 18 | 21 | 25 | 28

## Efektyvumo analizė ##

Tyrimui naudojamas `kursiokai100000.txt` failas, kuriame saugomi 100000 studentų duomenų. Grupavimui naudojama greičiausia (3 strategija).

Įvedimas | Išvedimas | Bendras laikas
------------ | ------------- | ------------- 
*std::vector* | 1.20s | 2.17s | 4.05s
*Vector* | 1.23s  | 2.14s | 4.03s

## Metodų pavyzdžiai ##

### size() ###

```
size_type size() const noexcept {return avail - itt;}

```

### capacity() ###

```
size_type capacity() const {return limit - itt;}
```

### shrink_to_fit() ###

```
void shrink_to_fit() {limit = avail;}
```

### begin() ###

```
iterator begin(){return itt;}
const_iterator begin() const {return itt;}
```

### back() ###

```
T& back()
{
   iterator it = avail;
   return *(--it);
}

const T& back() const
{
   iterator it = avail;
   return *(--it);
}

```
