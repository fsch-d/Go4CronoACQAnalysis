// $Id$
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#include "TCronoACQEventSource.h"

#include "TClass.h"
#include "TSocket.h"
#include "Bytes.h"


#include "TGo4Log.h"
#include "TGo4EventErrorException.h"
#include "TGo4EventEndException.h"
#include "TGo4UserSourceParameter.h"
#include "TCronoACQRawEvent.h"

#include <fftw3.h>




TCronoACQEventSource::TCronoACQEventSource(const char *name,
											const char *args,
											Int_t port) :
	TGo4EventSource(name),
	fbIsOpen(kFALSE),
	fxArgs(args),
	fiPort(port)
{
	Open();
}

TCronoACQEventSource::TCronoACQEventSource(TGo4UserSourceParameter* par) :
	TGo4EventSource(" "),
	fbIsOpen(kFALSE),
	fxArgs(),
	fiPort(0)
{
	if(par) {
		SetName(par->GetName());
		SetPort(par->GetPort());
		SetArgs(par->GetExpression());
		Open();
	} else {
		TGo4Log::Error("TCronoACQEventSource constructor with zero parameter!");
	}
}

TCronoACQEventSource::TCronoACQEventSource() :
	TGo4EventSource("default CronoACQ source"),
	fbIsOpen(kFALSE),
	fxArgs(),
	fiPort(0)
{
}

TCronoACQEventSource::~TCronoACQEventSource()
{
	Close();
}

Bool_t TCronoACQEventSource::CheckEventClass(TClass *cl)
{
	return cl->InheritsFrom(TCronoACQRawEvent::Class());
}

Bool_t TCronoACQEventSource::BuildEvent(TGo4EventElement *dest)
{
	TCronoACQRawEvent* evnt = (TCronoACQRawEvent*) dest;
	if (!evnt) return kFALSE;


	/////////////////////////////////////////////////////////////
	//////\brief read from file
	////////////////////////////////////////////////////////////
	if(daqInputType == 1 || daqInputType == 2){ //read from file
			typedef struct {
				Int_t trigger_index;
				Int_t flags;
				Long64_t timestamp;
				Int_t packet_count;
			} crono_group;
			
			typedef struct {
				UChar_t channel;
				UChar_t card;
				UChar_t type;
				UChar_t flags;
				UInt_t length;
				Long64_t timestamp;
			} crono_packet ;
			
			crono_group group,*p_group;
			crono_packet packet,*p_packet;
			p_group=&group;
			p_packet=&packet;
			Long64_t packet_time;
//			Int_t threshold=1000;//1128;
			Int_t status=1;
			Int_t channel=-1;
			Short_t data_buf[10240];//note the packet data should be smaller than that.
			Int_t event_count = 0;

           
	   // read another event from open file into our buffer
			if(fxFile->eof() || !fxFile->good()) {
				// reached last line or read error?
				SetCreateStatus(1);
				SetErrMess(Form("End of input file %s", GetName()));
				SetEventStatus(1);
				Close();
				throw TGo4EventEndException(this);
			}

			if(daqInputType == 1) //only for daqb files
			{
				fxFile->read((char *) p_group, 20); // read group header
				evnt->groupTime=p_group->timestamp;
			}

			Int_t j = 0;
			while(1){
				if(daqInputType == 1 && j>=p_group->packet_count) break;

				fxFile->read((char *) p_packet, 16); // read packet header
				if(daqInputType == 2 && p_packet->flags == 129) //end of buffer for daqc files 
				{
					evnt->groupTime=p_packet->timestamp;
					break;
				}

				packet_time=p_packet->timestamp;
				channel=p_packet->card*5+p_packet->channel;
				if(p_packet->channel==4)
				{
					fxFile->read((char *)data_buf, 8 * p_packet->length);
					
					if (event_count < NUM_SIG_MAX) {
						evnt->fiTDC[event_count].channel = channel;
						evnt->fiTDC[event_count].value = packet_time;
						event_count++;
					}
					status=0;
				}
				else
				{
					Double_t found_time;

					fxFile->read((char *) data_buf, 8*p_packet->length); // read packet data
					
					//time_alternativeCF(data_buf, p_packet->length * 4);
					Double_t *filteredSignal = new Double_t[p_packet->length * 4];
					ChebychevFilter(data_buf, p_packet->length * 4, filteredSignal, 100., 0.5); //filters the raw signal and returns filteredSignal
					found_time = time_CF(filteredSignal, p_packet->length * 4);
					delete[] filteredSignal;
					
					if (event_count < NUM_SIG_MAX && found_time>0) {
						evnt->fiTDC[event_count].channel = channel;
						evnt->fiTDC[event_count].value = packet_time + found_time;
						event_count++;
					}
					status = 0;
				}
				j++;

			}
			if(daqInputType == 2)
			{
				for(Int_t j = 0; j < event_count; j++) evnt->fiTDC[j].value -= evnt->groupTime;
			}
	   // test here for error in input event

	   if(status!=0) {
		  evnt->SetValid(kFALSE);
		  // somethings wrong, display error message from f_evt_error()
		  SetErrMess("CronoACQ Event Source --  ERROR !!!");
		  throw TGo4EventErrorException(this);
		  event_count = 0;
	   }
	   else evnt->SetValid(kTRUE);
	   evnt->event_count = event_count;
	   return kTRUE;
	}


	/////////////////////////////////////////////////////////////
	//////\brief read server
	////////////////////////////////////////////////////////////

	else if(daqInputType == 0)
	{
     typedef struct {
          Long64_t channel;
          Long64_t value;
      }  bufferEvnt;

        Int_t event_count=0;
        Long64_t size;
        Int_t flag1=sock->RecvRaw(&size,8);

        if(flag1==-1)
        {
            // reached last line or read error?
            SetCreateStatus(1);
            SetErrMess(Form("End of net server %s", GetName()));
            SetEventStatus(1);
            throw TGo4EventErrorException(this);
        }
        size = net2host(size);
        int nos = size / sizeof(bufferEvnt);
        bufferEvnt* buffer = new bufferEvnt[nos];
        flag1=sock->RecvRaw(buffer,size);
        if(flag1==-1)
        {
            // reached last line or read error?
            SetCreateStatus(1);
            SetErrMess(Form("End of net server %s", GetName()));
            SetEventStatus(1);
            throw TGo4EventErrorException(this);
        }
        //cout << "channel:" /*<< std::hex*/ << net2host(buffer[nos-1].channel) << " time: " << net2host(buffer[nos-1].value) << endl;

        evnt->groupTime = net2host(buffer[0].value);
        for(int i=1; i < nos && i < NUM_SIG_MAX; i++)
        {
            evnt->fiTDC[event_count].value = net2host(buffer[i].value) - evnt->groupTime;
            evnt->fiTDC[event_count].channel = net2host(buffer[i].channel);
            event_count++;
        }

        evnt->event_count = event_count;
        evnt->SetValid(kTRUE);

        delete [] buffer;

        return kTRUE;
	}
	else return kFALSE;

	return kTRUE;
}


Int_t TCronoACQEventSource::Open()
{
	if(fbIsOpen) return -1;
	TGo4Log::Info("Open of TCronoACQEventSource %s", GetName());
	// open connection/file

	TString fn = GetName();

	if(fn.EndsWith("daqb"))
	{
		TGo4Log::Info("Open daqb file %s", GetName());
 		daqInputType = 1;
	}
	else if(fn.EndsWith("daqc"))
	{
		TGo4Log::Info("Open daqc file %s", GetName());
		daqInputType = 2;
	}
	else
	{
		TGo4Log::Info("Connect to server %s", GetName());
		daqInputType = 0;
	}


	if(daqInputType == 1 || daqInputType == 2){
		fxFile = new std::ifstream(GetName(),std::ios::binary);
		if(!fxFile || !fxFile->good()) {
			delete fxFile;
			fxFile = nullptr;
			TString errmsg = TString::Format("Eror opening user file:%s",GetName());
			SetCreateStatus(1);
			SetErrMess(errmsg.Data());
			throw TGo4EventErrorException(this);
		}
		fbIsOpen = kTRUE;
		return 0;
	}
	if(daqInputType == 0){
		if(fbIsConnected)  return -1;
    	sock = new TSocket(GetName(), 6005);
		fbIsConnected=kTRUE;
		fbIsOpen = kTRUE;
		return 0;
	}
	return -1;
}

Int_t TCronoACQEventSource::Close()
{
	if(!fbIsOpen) return -1;
	TGo4Log::Info("Close of TCronoACQEventSource");
	Int_t status = 0; // closestatus of source
	if(daqInputType == 1 || daqInputType == 2){
		if (fxFile) {
			delete fxFile;
			fxFile = nullptr;
		}
		daqInputType = -1;
	}
	if(daqInputType == 0){
		if(sock){
			sock->Close();
			delete sock;
			sock = nullptr;
		}
		fbIsConnected=kFALSE;
		daqInputType = -1;
	}
	fbIsOpen = kFALSE;
	return status;
}


Long64_t TCronoACQEventSource::time_CF(Short_t *data_buf, Int_t size)
{
	int max = 0, imax = 0;
	Float_t threshold = 0;
	//cout<<"time_cf"<<endl;
	for (int i = 0; i < size; i++)
	{//cout<<"data "<<i<<endl;

		if (abs(data_buf[i]) > max)
		{
			max = abs(data_buf[i]);
			//cout<<"max "<<max<<endl;
			imax = i;
		}
	}
	Double_t found_time = 0;

	threshold = max * 0.7;//cout<<"*** buffer end "<<endl;
						//found_time=time_threshold(data_buf, size, threshold);
	for (int j = imax; j >= 0; j--)
	{
		if (abs(data_buf[j - 1]) < threshold&&abs(data_buf[j]) >= threshold)
		{
			found_time = (800.0*((float)j - 1 + (-fabs((float)data_buf[j - 1]) + (float)threshold) / (fabs((float)data_buf[j]) - fabs((float)data_buf[j - 1]))));
			//found_time = 800.0*(float)imax;
			break;
			//cout<<"time is found for packet "<<j<<"channel "<<channel<<"time "<<evnt->fiTdc[channel][hit[channel]-1]<<endl;
		}
	}
	return (Long64_t)found_time;// (data_buf[imax-1]*(imax-1)+data_buf[imax]*imax+data_buf[imax+1]*(imax+1))/(data_buf[imax-1]+data_buf[imax]+data_buf[imax+1])*800;//found_time;
}

Long64_t TCronoACQEventSource::time_CF(Double_t *signal, Int_t size)
{
	int imax = 0;
	Double_t threshold = 0, max = 0;
	Double_t found_time = 0;
	for (int i = 0; i < size; i++) {
		if (abs(signal[i]) > max)
		{
			max = abs(signal[i]);
			imax = i;
		}
	}
	


	threshold = max * 0.7;
	for (int j = imax; j >= 0; j--)
	{
		if (abs(signal[j - 1]) < threshold&&abs(signal[j]) >= threshold)
		{
			found_time = (800.0*((Double_t)j - 1. + (-fabs(signal[j - 1]) + threshold) / (fabs(signal[j]) - fabs(signal[j - 1]))));
			break;
		}
	}
	
	return (Long64_t)found_time;// (data_buf[imax-1]*(imax-1)+data_buf[imax]*imax+data_buf[imax+1]*(imax+1))/(data_buf[imax-1]+data_buf[imax]+data_buf[imax+1])*800;//found_time;
}

void TCronoACQEventSource::ChebychevFilter(Short_t *data_buf, Int_t size, Double_t *filteredSignal, Double_t f0, Double_t ripple)
{
	//f0 is cutoff frequency in MHz
	Double_t fs = 1250; //is ADC sampling frequency in MHz, for cronologic (ABCD) 1250;

	Double_t *in, *out2;
	fftw_complex *out;
	fftw_plan fft_forward, fft_backward;

	in = (Double_t*)fftw_malloc(sizeof(Double_t) * size);
	out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (size / 2 + 1));
	out2 = (Double_t*)fftw_malloc(sizeof(Double_t) * size);



	for (Int_t i = 0; i < size; i++) {
		in[i] = Double_t(data_buf[i]);
	//	cout << in[i] << "\t";
	}
//	cout << endl;

	fft_forward = fftw_plan_dft_r2c_1d(size, in, out, FFTW_ESTIMATE);
	fftw_execute(fft_forward);

	//remove DC offset

	for (Int_t i = 1; i < size/2+1; i++) {

		Double_t x = fs/size*i;
		Double_t G;
		if(x>0) G = 1 / sqrt(1 + (pow(ripple, 2)*pow(16 * pow(x / f0, 5) - 20 * pow(x / f0, 3) + 5 * (x / f0), 2))); //5th order Chebishev polynomial
		else G = 0;
		out[i][0] = G * out[i][0];
		out[i][1] = G * out[i][1];
	}
	fft_backward = fftw_plan_dft_c2r_1d(size, out, out2, FFTW_ESTIMATE);
	fftw_execute(fft_backward);

	//


	for (Int_t i = 0; i < size; i++) {
		filteredSignal[i]=out2[i]/size;
		//cout << filteredSignal[i] << "\t";
	}
	//cout << endl;

	fftw_destroy_plan(fft_forward);
	fftw_destroy_plan(fft_backward);
	fftw_free(in);
	fftw_free(out);
	fftw_free(out2);
	return;
}
