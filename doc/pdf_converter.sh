cd latex
pdflatex refman.tex
mv refman.pdf ..
cd ..
rm -r latex/
mkdir latex
mv refman.pdf latex