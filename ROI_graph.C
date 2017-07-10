
//-------------------------------------------------------------------------------------------------------------------------------------------------------
// ROI_graph
//                  created by Kazuo Tanaka
//-------------------------------------------------------------------------------------------------------------------------------------------------------


//////////////////////////////////////////////////////////////////
//include
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>







#ifndef ROOT_TGDockableFrame
#include "TGDockableFrame.h"
#endif
#ifndef ROOT_TGMdiDecorFrame
#include "TGMdiDecorFrame.h"
#endif
#ifndef ROOT_TG3DLine
#include "TG3DLine.h"
#endif
#ifndef ROOT_TGMdiFrame
#include "TGMdiFrame.h"
#endif
#ifndef ROOT_TGedFrame
#include "TGedFrame.h"
#endif
#ifndef ROOT_TGMdiMainFrame
#include "TGMdiMainFrame.h"
#endif
#ifndef ROOT_TGMdiMenu
#include "TGMdiMenu.h"
#endif
#ifndef ROOT_TGListBox
#include "TGListBox.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGScrollBar
#include "TGScrollBar.h"
#endif
#ifndef ROOT_TGComboBox
#include "TGComboBox.h"
#endif
#ifndef ROOT_TGFrame
#include "TGFrame.h"
#endif
#ifndef ROOT_TGMenu
#include "TGMenu.h"
#endif
#ifndef ROOT_TGedEditor
#include "TGedEditor.h"
#endif
#ifndef ROOT_TGFileDialog
#include "TGFileDialog.h"
#endif
#ifndef ROOT_TPadEditor
#include "TPadEditor.h"
#endif
#ifndef ROOT_TGShutter
#include "TGShutter.h"
#endif
#ifndef ROOT_TGButtonGroup
#include "TGButtonGroup.h"
#endif
#ifndef ROOT_TRootDialog
#include "TRootDialog.h"
#endif
#ifndef ROOT_TGedPatternSelect
#include "TGedPatternSelect.h"
#endif
#ifndef ROOT_TGCanvas
#include "TGCanvas.h"
#endif
#ifndef ROOT_TGFSContainer
#include "TGFSContainer.h"
#endif

#ifndef ROOT_TGColorSelect
#include "TGColorSelect.h"
#endif
#ifndef ROOT_TGButton
#include "TGButton.h"
#endif
#ifndef ROOT_TRootContextMenu
#include "TRootContextMenu.h"
#endif
#ifndef ROOT_TGSlider
#include "TGSlider.h"
#endif
#ifndef ROOT_TGFSComboBox
#include "TGFSComboBox.h"
#endif
#ifndef ROOT_TGLabel
#include "TGLabel.h"
#endif
#ifndef ROOT_TGMsgBox
#include "TGMsgBox.h"
#endif

#ifndef ROOT_TGTab
#include "TGTab.h"
#endif
#ifndef ROOT_TGListView
#include "TGListView.h"
#endif
#ifndef ROOT_TGSplitter
#include "TGSplitter.h"
#endif
#ifndef ROOT_TRootCanvas
#include "TRootCanvas.h"
#endif
#ifndef ROOT_TGStatusBar
#include "TGStatusBar.h"
#endif
#ifndef ROOT_TAttLineEditor
#include "TAttLineEditor.h"
#endif
#ifndef ROOT_TGListTree
#include "TGListTree.h"
#endif
#ifndef ROOT_TAttFillEditor
#include "TAttFillEditor.h"
#endif
#ifndef ROOT_TGNumberEntry
#include "TGNumberEntry.h"
#endif
#ifndef ROOT_TGToolTip
#include "TGToolTip.h"
#endif
#ifndef ROOT_TGToolBar
#include "TGToolBar.h"
#endif
#ifndef ROOT_TRootEmbeddedCanvas
#include "TRootEmbeddedCanvas.h"
#endif
#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif


#include "Riostream.h"






const int lifetime = 180;



//設定をクリア
gROOT->Reset();
//ファイル読み込み関連変数設定
char line[500];
char *ptr;
int i = 0;
double array_x[30000];
double array_y[30000];
int rb_fr = 1;
double buf[500];
int interval = 1000;
int start = 0;

TGMainFrame *fMainFrame874 = new TGMainFrame(gClient->GetRoot(), 10, 10, kMainFrame | kVerticalFrame);
TRootEmbeddedCanvas *cem1 = new TRootEmbeddedCanvas(0, fMainFrame874, 472, 320);
Int_t wcem1 = cem1->GetCanvasWindowId();
TCanvas *c123 = new TCanvas("c123", 10, 10, wcem1);
	TGNumberEntry *t1 = new TGNumberEntry(fMainFrame874, (Double_t) 0, 8, -1, (TGNumberFormat::EStyle) 5);
TF1 f1("f1", "[1]*(1-exp(0.693*(-(x-[2]))/190))+[3]");


f1.SetParameter(0, 1000);
f1.SetParameter(1, 1);
f1.SetParameter(2, 1000);
int ROI_graph(int rb_fr_arg, int interval_arg) {
	rb_fr = rb_fr_arg;
	interval = interval_arg*1000;
	// main frame

	fMainFrame874->SetName("fMainFrame874");
	fMainFrame874->SetLayoutBroken(kTRUE);

	t1->SetName("t1");
	fMainFrame874->AddFrame(t1, new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 2, 2, 2));
	t1->MoveResize(0, 536, 100, 24);


	//ok button
   TGTextButton *submit = new TGTextButton(fMainFrame874,"OK",-1,TGTextButton::GetDefaultGC()(),TGTextButton::GetDefaultFontStruct(),kRaisedFrame);
   submit->SetTextJustify(36);
   submit->SetMargins(0,0,0,0);
   submit->SetWrapLength(-1);
   submit->Resize(59,24);
   fMainFrame874->AddFrame(submit, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
   submit->MoveResize(424,536,59,24);
	submit->Connect("Clicked()","MyMainFrame",0,"t1Update()");
	// embedded canvas
	cem1->SetName("cem1");
	cem1->AdoptCanvas(c123);
	fMainFrame874->AddFrame(cem1, new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 2, 2, 2));
	cem1->MoveResize(8, 8, 672, 520);

	fMainFrame874->SetMWMHints(kMWMDecorAll,
	                           kMWMFuncAll,
	                           kMWMInputModeless);
	fMainFrame874->MapSubwindows();

	fMainFrame874->Resize(fMainFrame874->GetDefaultSize());
	fMainFrame874->MapWindow();
	fMainFrame874->Resize(690, 572);
	fMainFrame874->Move(1000, 200);

//Canvasの設定
//  c1 = new TCanvas("c1","chr_graph_title",200,10,700,500);
	c123->SetGrid();
	c123->GetFrame()->SetFillColor(0);
	c123->GetFrame()->SetBorderSize(12);
//プロットのための整形処理
	TGraph* tg = new TGraph();
	tg->SetTitle("ROI graph");
	tg->GetXaxis()->SetTitle("time (s)");
	tg->GetYaxis()->SetTitle("ROI");
	tg->GetYaxis()->SetTitleOffset(1.3);



//プロット

	TTimer *timer = new TTimer(interval);
	timer->SetCommand("Update(rb_fr)");
	timer->TurnOn();


//////////////////////////////////////////////////////////////////



	return 0;
}
void Update(int rb_fr) {
//////////////////////////////////////////////////////////////////


	array_x[0] = 0; array_y[0] = 0;
	TGraph* tg = new TGraph(1, array_x, array_y);
	tg->Draw("AL");

	int array_size_temp = 0;
	FILE *fp = fopen("ROI.txt", "r");
	//data fileから値を読み込んでtreeへfill
			if (fp == NULL) {
	//	        exit(1);	
		        	}
	while (fgets(&line, 500, fp)) {

		ptr = line;
		i = 0;
		do {
			// lineから次のタブ文字までを数値に変換 */
			buf[i] = atof(ptr);
			// 次のタブ文字の位置を探す */
			ptr = strchr(ptr, '\t');
			// タブ文字の次の文字を示す */
			if (ptr != NULL) {
				ptr++;
				//x列かy列が欠けていたら次の行へ。
				if (i < 0 && i < 1) {
					goto label1;
				}
			}
			i++;
		} while (ptr != NULL);

		array_x[array_size_temp] = buf[0];
		array_y[array_size_temp] = buf[1];
		array_size_temp++;
label1:
	}
	//TGraphのインスタンス作成
	TGraph* tg = new TGraph(array_size_temp, array_x, array_y);
	tg->Draw("AL");
	if (rb_fr == 1) {
		tg->Fit("f1", "", "", start, array_x[array_size_temp-1]);

	}

  cout <<"start:" <<start <<"\t end " <<array_x[array_size_temp-1] <<"\n";
	c123->Update();
}
void t1Update(){
	start=t1->GetNumber();
	cout << "change \n";
}