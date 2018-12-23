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


void AReadFromTade::setRuns( vector<run_parameter *> selectedRuns )
{
    for(unsigned int i=0;i<selectedRuns.size();i++)
    {
	vector<string> tmp;
	vector<int> itmp;
	comboBox1->insertItem(selectedRuns[i]->getName().data());
	for(int j=0;j<selectedRuns[i]->getNumberOfFiles();j++)
	{
	    tmp.push_back(selectedRuns[i]->getFile(j).data());
	    itmp.push_back(selectedRuns[i]->getFileType(j));
	}
	fileNames.push_back(tmp);
	fileTypes.push_back(itmp);
    }
    if(comboBox1->count()<1)return;
    comboBox1->setCurrentItem(0);
    
}


