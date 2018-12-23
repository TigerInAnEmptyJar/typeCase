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
void doc_selectClassesDialog::setClasses(const vector<TMyClass> &classes)
{
    listBox1->clear();
    listBox2->clear();
    for(unsigned int i=0;i<classes.size();i++)
#if QT_VERSION < 0x040000
	listBox1->insertItem(classes[i].Name().data());
#else
	listBox1->addItem(classes[i].Name().data());
#endif
}
void doc_selectClassesDialog::getClasses(vector<string> &ret)
{
    for(int i=0;i<(int)listBox2->count();i++)
      ret.push_back(QS2s(listBox2->item(i)->text()));
}
void doc_selectClassesDialog::OnUseButtonClick()
{
#if QT_VERSION < 0x040000
    int num=listBox1->currentItem();
    listBox2->insertItem(listBox1->text(num));
    listBox1->removeItem(num);
#else
    int num=listBox1->currentRow();
    listBox2->addItem(listBox1->item(num)->text());
    delete listBox1->takeItem(num);
#endif
}
void doc_selectClassesDialog::OnUnUseButtonClick()
{
#if QT_VERSION < 0x040000
    int num=listBox2->currentItem();
    listBox1->insertItem(listBox2->text(num));
    listBox2->removeItem(num);
#else
    int num=listBox2->currentRow();
    listBox1->addItem(listBox2->item(num)->text());
    delete listBox2->takeItem(num);
#endif
}
