// -*- C++ -*-
// $Id$

#include "Queued_Message.h"
#include "Message_Sent_Callback.h"

#if !defined (__ACE_INLINE__)
# include "Queued_Message.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Queued_Message, "$Id$")

TAO_Queued_Message::TAO_Queued_Message (ACE_Message_Block *contents,
                                        TAO_Message_Sent_Callback *callback)
  : contents_ (contents)
  , callback_ (callback)
  , next_ (0)
  , prev_ (0)
{
}

TAO_Queued_Message::~TAO_Queued_Message (void)
{
  ACE_Message_Block::release (this->contents_);
}

void
TAO_Queued_Message::connection_closed (void)
{
  if (this->callback_ != 0)
    this->callback_->connection_closed ();
}

void
TAO_Queued_Message::destroy (void)
{
  delete this;
}

void
TAO_Queued_Message::bytes_transferred (size_t byte_count)
{
  while (byte_count > 0 && !this->done ())
    {
      size_t l = this->contents_->length ();
      if (byte_count < l)
        {
          this->contents_->rd_ptr (byte_count);
          return;
        }
      ACE_Message_Block *cont = this->contents_->cont ();
      byte_count -= l;
      ACE_Message_Block::release (this->contents_);
      this->contents_ = cont;
    }
}

void
TAO_Queued_Message::remove_from_list (TAO_Queued_Message *&head,
                                      TAO_Queued_Message *&tail)
{
  if (this->prev_ != 0)
    this->prev_->next_ = this->next_;
  else
    head = this->next_;

  if (this->next_ != 0)
    this->next_->prev_ = this->prev_;
  else
    tail = this->prev_;

  this->next_ = 0;
  this->prev_ = 0;
}

void
TAO_Queued_Message::push_back (TAO_Queued_Message *&head,
                               TAO_Queued_Message *&tail)
{
  if (tail == 0)
    {
      tail = this;
      head = this;
      this->next_ = 0;
      this->prev_ = 0;
      return;
    }

  this->prev_ = tail;
  this->next_ = 0;
  if (tail->prev_ != 0)
    {
      tail->prev_->next_ = this;
    }
  tail = this;
}
