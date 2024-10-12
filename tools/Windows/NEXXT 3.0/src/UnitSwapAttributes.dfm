inherited FormSwapAttributes: TFormSwapAttributes
  Caption = 'Swap Attributes / Subpalettes'
  PixelsPerInch = 96
  TextHeight = 16
  inherited GroupBox1: TGroupBox
    inherited GroupBox4: TGroupBox
      Caption = 'Apply to subpalette sets'
      inherited RadioPalAll: TRadioButton
        Left = 13
        Top = 42
        ExplicitLeft = 13
        ExplicitTop = 42
      end
      inherited RadioPalOne: TRadioButton
        Left = 118
        Width = 81
        Caption = 'other sets'
        ExplicitLeft = 118
        ExplicitWidth = 81
      end
      inherited RadioPalNone: TRadioButton
        Top = 21
        ExplicitTop = 21
      end
      inherited ButtonWhichSubpal: TButton
        Left = 180
        Top = 17
        Caption = 'n/a6'
        Visible = False
        ExplicitLeft = 180
        ExplicitTop = 17
      end
    end
    inherited GroupBox3: TGroupBox
      Caption = 'Attribute Order'
    end
    inherited GroupBox2: TGroupBox
      Caption = 'Apply to attribute table'
    end
    inherited RadioButton4K: TRadioButton
      Caption = 'entire table'
    end
    inherited RadioButton8K: TRadioButton
      Top = 104
      Width = 53
      Caption = 'n/a4'
      Visible = False
      ExplicitTop = 104
      ExplicitWidth = 53
    end
    inherited ButtonDarker: TButton
      Left = 248
      Width = 6
      Caption = 'n/a2'
      Visible = False
      ExplicitLeft = 248
      ExplicitWidth = 6
    end
    inherited ButtonBrighter: TButton
      Left = 286
      Width = 6
      Caption = 'n/a3'
      Visible = False
      ExplicitLeft = 286
      ExplicitWidth = 6
    end
    inherited RadioPatternNone: TRadioButton
      Top = 84
      ExplicitTop = 84
    end
    inherited Button5: TButton
      Left = 276
      Top = 48
      Width = 4
      Caption = 'n/a1'
      ExplicitLeft = 276
      ExplicitTop = 48
      ExplicitWidth = 4
    end
    inherited Button6: TButton
      Left = 147
      Width = 68
      Caption = 'Flip order'
      ExplicitLeft = 147
      ExplicitWidth = 68
    end
    inherited CheckBox1: TCheckBox
      Top = 48
      ExplicitTop = 48
    end
    inherited GroupBox5: TGroupBox
      Height = 62
      Caption = 'to sprites'
      ExplicitHeight = 62
      inherited ButtonCol0: TButton
        Left = 65
        Width = 3
        Caption = 'n/a8'
        Visible = False
        ExplicitLeft = 65
        ExplicitWidth = 3
      end
      object RadioSpritesAll: TRadioButton
        Left = 13
        Top = 19
        Width = 40
        Height = 17
        Caption = 'all'
        Checked = True
        TabOrder = 1
        TabStop = True
        OnClick = CheckBox1Click
      end
      object RadioSpritesNone: TRadioButton
        Left = 13
        Top = 38
        Width = 50
        Height = 17
        Caption = 'none'
        TabOrder = 2
        OnClick = CheckBox1Click
      end
    end
  end
end
