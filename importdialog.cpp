/*                                                                      
    This file is part of KAddressBook.                                  
    Copyright (c) 2002 Mike Pilone <mpilone@slac.com>                   
                                                                        
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or   
    (at your option) any later version.                                 
                                                                        
    This program is distributed in the hope that it will be useful,     
    but WITHOUT ANY WARRANTY; without even the implied warranty of      
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        
    GNU General Public License for more details.                        
                                                                        
    You should have received a copy of the GNU General Public License   
    along with this program; if not, write to the Free Software         
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.           
                                                                        
    As a special exception, permission is given to link this program    
    with any edition of Qt, and distribute the resulting executable,    
    without including the source code for Qt in the source distribution.
*/                                                                      

#include "importdialog.h"

#include <kdebug.h>
#include <klocale.h>

#include <kabc/addressbook.h>
#include <kabc/addressee.h>
#include <kabc/phonenumber.h>
#include <kabc/address.h>

ContactImportDialog::ContactImportDialog(KABC::AddressBook *doc,
                                         QWidget *parent)
        : KImportDialog(parent), mDocument(doc)
{
    mCustomList.setAutoDelete( true );

    mFormattedName = new KImportColumn(this, KABC::Addressee::formattedNameLabel());
    mLastName = new KImportColumn(this, KABC::Addressee::familyNameLabel(), 1);
    mFirstName = new KImportColumn(this, KABC::Addressee::givenNameLabel(), 1);
    mAdditionalName = new KImportColumn(this, KABC::Addressee::additionalNameLabel());
    mNamePrefix = new KImportColumn(this, KABC::Addressee::prefixLabel());
    mNameSuffix = new KImportColumn(this, KABC::Addressee::suffixLabel());
    mNickName = new KImportColumn(this, KABC::Addressee::nickNameLabel());
    mBirthday = new KImportColumn(this, KABC::Addressee::birthdayLabel());

    mAddressHomeStreet = new KImportColumn(this, KABC::Addressee::homeAddressStreetLabel());
    mAddressHomeCity = new KImportColumn(this, KABC::Addressee::homeAddressLocalityLabel());
    mAddressHomeState = new KImportColumn(this, KABC::Addressee::homeAddressRegionLabel());
    mAddressHomeZip = new KImportColumn(this, KABC::Addressee::homeAddressPostalCodeLabel());
    mAddressHomeCountry = new KImportColumn(this, KABC::Addressee::homeAddressCountryLabel());
    mAddressHomeLabel = new KImportColumn(this, KABC::Addressee::homeAddressLabelLabel());

    mAddressBusinessStreet = new KImportColumn(this, KABC::Addressee::businessAddressStreetLabel());
    mAddressBusinessCity = new KImportColumn(this, KABC::Addressee::businessAddressLocalityLabel());
    mAddressBusinessState = new KImportColumn(this, KABC::Addressee::businessAddressRegionLabel());
    mAddressBusinessZip = new KImportColumn(this, KABC::Addressee::businessAddressPostalCodeLabel());
    mAddressBusinessCountry = new KImportColumn(this, KABC::Addressee::businessAddressCountryLabel());
    mAddressBusinessLabel = new KImportColumn(this, KABC::Addressee::businessAddressLabelLabel());

    mPhoneHome = new KImportColumn(this, KABC::Addressee::homePhoneLabel());
    mPhoneBusiness = new KImportColumn(this, KABC::Addressee::businessPhoneLabel());
    mPhoneMobile = new KImportColumn(this, KABC::Addressee::mobilePhoneLabel());
    mFaxHome = new KImportColumn(this, KABC::Addressee::homeFaxLabel());
    mFaxBusiness = new KImportColumn(this, KABC::Addressee::businessFaxLabel());
    mCarPhone = new KImportColumn(this, KABC::Addressee::carPhoneLabel());
    mIsdn = new KImportColumn(this, KABC::Addressee::isdnLabel());
    mPager = new KImportColumn(this, KABC::Addressee::pagerLabel());
    mEmail = new KImportColumn(this, KABC::Addressee::emailLabel());
    mMailClient = new KImportColumn(this, KABC::Addressee::mailerLabel());
    mJobTitle = new KImportColumn(this, KABC::Addressee::titleLabel());
    mRole = new KImportColumn(this, KABC::Addressee::roleLabel());
    mCompany = new KImportColumn(this, KABC::Addressee::organizationLabel());
    mNote = new KImportColumn(this, KABC::Addressee::noteLabel());
    mUrl = new KImportColumn(this, KABC::Addressee::urlLabel());

    KABC::Field::List fields = mDocument->fields( KABC::Field::CustomCategory );
    KABC::Field::List::Iterator it;

    for ( it = fields.begin(); it != fields.end(); ++it ) {
      KImportColumn *column = new KImportColumn( this, (*it)->label() );
      mCustomList.append( column );
    }

    registerColumns();
}

void ContactImportDialog::convertRow()
{
  KABC::Addressee a;
  a.setFormattedName(mFormattedName->convert());
  a.setGivenName(mFirstName->convert());
  a.setFamilyName(mLastName->convert());
  a.setAdditionalName(mAdditionalName->convert());
  a.setPrefix(mNamePrefix->convert());
  a.setSuffix(mNameSuffix->convert());
  a.setNickName(mNickName->convert());
  a.setBirthday( QDateTime::fromString( mBirthday->convert(), Qt::ISODate ) );
  a.insertEmail(mEmail->convert(), true);
  a.setRole(mRole->convert());
  a.setTitle(mJobTitle->convert());
  a.setMailer(mMailClient->convert());
  a.setUrl(mUrl->convert());

  KABC::PhoneNumber p(mPhoneBusiness->convert(), KABC::PhoneNumber::Work);
  a.insertPhoneNumber(p);
  
  p.setNumber(mPhoneHome->convert());
  p.setType(KABC::PhoneNumber::Home);
  a.insertPhoneNumber(p);
  
  p.setNumber(mPhoneMobile->convert());
  p.setType(KABC::PhoneNumber::Cell);
  a.insertPhoneNumber(p);
  
  p.setNumber(mFaxHome->convert());
  p.setType(KABC::PhoneNumber::Home | KABC::PhoneNumber::Fax);
  a.insertPhoneNumber(p);
  
  p.setNumber(mFaxBusiness->convert());
  p.setType(KABC::PhoneNumber::Work | KABC::PhoneNumber::Fax);
  a.insertPhoneNumber(p);

  p.setNumber(mCarPhone->convert());
  p.setType(KABC::PhoneNumber::Car);
  a.insertPhoneNumber(p);

  p.setNumber(mIsdn->convert());
  p.setType(KABC::PhoneNumber::Isdn);
  a.insertPhoneNumber(p);

  p.setNumber(mPager->convert());
  p.setType(KABC::PhoneNumber::Pager);
  a.insertPhoneNumber(p);
  
  a.setOrganization(mCompany->convert());
  a.setNote(mNote->convert());
  
  KABC::Address addrHome(KABC::Address::Home);
  addrHome.setStreet(mAddressHomeStreet->convert());
  addrHome.setLocality(mAddressHomeCity->convert());
  addrHome.setRegion(mAddressHomeState->convert());
  addrHome.setPostalCode(mAddressHomeZip->convert());
  addrHome.setCountry(mAddressHomeCountry->convert());
  addrHome.setLabel(mAddressHomeLabel->convert());
  a.insertAddress(addrHome);
  
  KABC::Address addrWork(KABC::Address::Work);
  addrWork.setStreet(mAddressBusinessStreet->convert());
  addrWork.setLocality(mAddressBusinessCity->convert());
  addrWork.setRegion(mAddressBusinessState->convert());
  addrWork.setPostalCode(mAddressBusinessZip->convert());
  addrWork.setCountry(mAddressBusinessCountry->convert());
  addrWork.setLabel(mAddressBusinessLabel->convert());
  a.insertAddress(addrWork);

  KABC::Field::List fields = mDocument->fields( KABC::Field::CustomCategory );
  KABC::Field::List::Iterator it;

  uint counter = 0;
  for ( it = fields.begin(); it != fields.end(); ++it ) {
    (*it)->setValue( a, mCustomList.at( counter )->convert() );
    ++counter;
  }
  
  mDocument->insertAddressee(a);
}

