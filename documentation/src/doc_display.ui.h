/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
void my_doc_display::showText(const vector<string>& text, TMyClass* cls)
{
  textEdit1->clear();
  for (unsigned int i = 0; i < text.size(); i++)
    textEdit1->append(text[i].data());
  displayedClass = cls;
}
void my_doc_display::showFile(const string& filename, TMyClass* cls)
{
  textEdit1->clear();
  ifstream inp;
  inp.open(filename.data());
  char li[1000];
  while (inp.good() && !inp.eof()) {
    inp.getline(li, 1000);
    textEdit1->append(li);
  }
  inp.close();
  displayedClass = cls;
}
void my_doc_display::setClass(TMyClass* cls) { displayedClass = cls; }
TMyClass* my_doc_display::getClass() { return displayedClass; }
